#include "Spartan2OEM.h"
#include "LookupTables.h"

/*
 * This Code example has been tested with Arduino Uno and Arduino Diecimila
 * SDA and SLC lines require 4.7k pullups
 * Connect Arduino SDA to Spartam-2-OEM-I2C SDA
 * Connect Arduino SLC to Spartam-2-OEM-I2C SCL
 * Data from Spartam-2-OEM-I2C is diplayed on the Arduino IDE Serial Monitor
 * If you need to force/reset Spartam-2-OEM-I2C's I2C Address,
 * please read the last page of the Spartam_2_OEM_I2C Manual
 */

const uint8_t Spartan2OEM::I2C_RESPONSE_LENGTH   = 6;
const uint8_t Spartan2OEM::I2C_RW_CMD            = 0;
const uint8_t Spartan2OEM::I2C_SLAVE_MIN_ADDRESS = 1;
const uint8_t Spartan2OEM::I2C_SLAVE_MAX_ADDRESS = 16;

Spartan2OEM::Spartan2OEM(TwoWire* i2c_bus, const uint8_t &i2c_address)
{
	this->i2c_bus     = i2c_bus;
	this->i2c_address = i2c_address;
	this->data_available = false;
	this->lambda = 0;
	this->temperature_c = 0;
	this->version_hs = 0;
	this->timeout_ms = 0;
}

bool Spartan2OEM::ChangeI2CAddress(const uint8_t &new_address)
{
	i2c_bus->beginTransmission(i2c_address); //Setup communication with device @ I2C Address = 1
	i2c_bus->write(I2C_RW_CMD);     //This will tell Spartan that OEM that we want to start a read from Memory Address 0
	i2c_bus->write(new_address);                //write New address to Spartan 2 OEM
	i2c_bus->endTransmission();

	i2c_address = new_address;

	return true;
}

bool Spartan2OEM::RequestData()
{
	i2c_bus->beginTransmission(i2c_address); //Setup communication with device @ I2C Address = 1
	i2c_bus->write(I2C_RW_CMD);         //This will tell Spartan that OEM that we want to start a read from Memory Address 0
	i2c_bus->endTransmission();              // end transmission

	//Read 6 Bytes from Device with I2C Address = I2C_Address
	i2c_bus->requestFrom(i2c_address, I2C_RESPONSE_LENGTH);

	// Clear rx buffer
	memset(rx_buffer, 0, sizeof(rx_buffer));

	uint8_t index = 0;
	while (i2c_bus->available() && index < I2C_RESPONSE_LENGTH) //Dump the I2C data into an Array
	{
		rx_buffer[index] = i2c_bus->read();
		++index;
	}

	data_available = (I2C_RESPONSE_LENGTH == index);
	return data_available;
}

void Spartan2OEM::ProcessData()
{
	if (data_available)
	{
		uint8_t i2c_addr        = rx_buffer[0];
		version_hs              = rx_buffer[1];
		uint16_t pump_current16 = (rx_buffer[2] << 8) + rx_buffer[3];
		uint8_t ri              = rx_buffer[4];
		uint8_t status8         = rx_buffer[5];

		lambda = LookupTables::GetLambda(pump_current16);
		temperature_c = LookupTables::GetTemperatureC(ri);

		data_available = false;
	}
}

float Spartan2OEM::Lambda() const
{
	return (float)lambda / (float)1000;
}

float Spartan2OEM::AfrRatio() const
{
	// 14.7 AFR = 1 lambda
	return Lambda() * 14.7;
}

uint16_t Spartan2OEM::TemperatureC() const
{
	return temperature_c;
}

//TODO: Check the type conversion
uint16_t Spartan2OEM::TemperatureF() const
{
	return temperature_c * 1.8 + 32;
}

uint8_t Spartan2OEM::Version() const
{
	return version_hs;
}

uint8_t Spartan2OEM::Status() const
{
	return 0;
}

void Spartan2OEM::SetTimeot(const uint16_t &millis)
{
	this->timeout_ms = millis;
}

uint16_t Spartan2OEM::Timeout() const
{
	return timeout_ms;
}
