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

//const uint8_t Spartan2OEM::I2C_RESPONSE_LENGTH   = 6;
//const uint8_t Spartan2OEM::I2C_RW_CMD            = 0;
//const uint8_t Spartan2OEM::I2C_SLAVE_ADDRESS_MIN = 1;
//const uint8_t Spartan2OEM::I2C_SLAVE_ADDRESS_MAX = 16;


Spartan2OEM::Spartan2OEM(TwoWire* i2c_bus, const uint8_t &i2c_address)
{
	this->i2c_bus     = i2c_bus;
	this->i2c_address = i2c_address;
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
	while (i2c_bus->available() & index < I2C_RESPONSE_LENGTH) //Dump the I2C data into an Array
	{
		rx_buffer[index] = i2c_bus->read();
		++index;
	}

	return (I2C_RESPONSE_LENGTH == index) ? true : false;
}

bool Spartan2OEM::ProcessData()
{
	uint8_t i2c_addr        = rx_buffer[0];
	uint8_t id8             = rx_buffer[1];
	uint16_t pump_current16 = (rx_buffer[2] << 8) + rx_buffer[3];
	uint8_t ri              = rx_buffer[4];
	uint8_t status8         = rx_buffer[5];

	uint8_t i = pump_current16 - Pump_Current16_to_Lambda_Start;
	if (i > (Pump_Current16_to_Lambda_Size - 1))
	{
		i = Pump_Current16_to_Lambda_Size - 1;
	}

	//use Pump_Current16 as index to Pump_Current16_to_Lambda lookup table and then Divide by 1000 to get actual Lambda
	float labda = ((float)pgm_read_word_near(Pump_Current16_to_Lambda + i)) / 1000;


	// Index has to be justified to the right by Ri_to_Temperature_C_Start
	i = ri - Ri_to_Temperature_C_Start;
	if (i > (Ri_to_Temperature_C_Size-1))
	{
		i = Ri_to_Temperature_C_Size-1;
	}

	uint16_t temperature_c = pgm_read_word_near(Ri_to_Temperature_C + i);

	return true;
}

float Spartan2OEM::Lambda() const
{

}

float Spartan2OEM::AfrRatio() const
{

}

uint16_t Spartan2OEM::TemperatureC() const
{

}

uint16_t Spartan2OEM::TemperatureF() const
{

}

uint8_t Spartan2OEM::Status() const
{

}

void Spartan2OEM::SetTimeot(const uint16_t &millis)
{
	this->timeoutMs = millis;
}

uint16_t Spartan2OEM::Timeout() const
{
	return timeoutMs;
}
