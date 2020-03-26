#include "Spartan2OEM.h"
#include "LookupTables.h"


const uint8_t Spartan2OEM::I2C_RESPONSE_LENGTH       = 6;
const uint8_t Spartan2OEM::I2C_RW_CMD                = 0;
const uint8_t Spartan2OEM::I2C_SLAVE_DEFAULT_ADDRESS = 0;
const uint8_t Spartan2OEM::I2C_SLAVE_MIN_ADDRESS     = 1;
const uint8_t Spartan2OEM::I2C_SLAVE_MAX_ADDRESS     = 16;

Spartan2OEM::Spartan2OEM()
{
}

void Spartan2OEM::Initialize(TwoWire* i2c_bus, const uint8_t &i2c_address)
{
	this->i2c_bus        = i2c_bus;
	this->i2c_address    = i2c_address;
	initialized = true;
}

bool Spartan2OEM::ChangeI2CAddress(const uint8_t &new_address)
{
	if (new_address >= I2C_SLAVE_MIN_ADDRESS && new_address <= I2C_SLAVE_MAX_ADDRESS)
	{
		i2c_bus->beginTransmission(I2C_SLAVE_DEFAULT_ADDRESS); // Setup communication with device @ I2C address
		i2c_bus->write(I2C_RW_CMD);                            // Tell Spartan2 OEM that we want to start a read from memory address 0
		i2c_bus->write(new_address);                           // Set new I2C address to Spartan2 OEM
		i2c_bus->endTransmission();

		i2c_address = new_address;

		return true;
	}

	return false;
}

bool Spartan2OEM::RequestData()
{
	if (initialized)
	{
		i2c_bus->beginTransmission(i2c_address); // Setup communication with device @ I2C Address
		i2c_bus->write(I2C_RW_CMD);              // Tell Spartan2 OEM that we want to start a read from memory address 0
		i2c_bus->endTransmission();              // End transmission

		// Request 6 bytes from Spartan2 OEM
		i2c_bus->requestFrom(i2c_address, I2C_RESPONSE_LENGTH);

		// Clear rx buffer
		memset(rx_buffer, 0, sizeof(rx_buffer));

		// Save data to rx buffer
		uint8_t received = 0;
		while (i2c_bus->available() && received < I2C_RESPONSE_LENGTH)
		{
			rx_buffer[received] = i2c_bus->read();
			++received;
		}

		data_available = (I2C_RESPONSE_LENGTH == received);
	}
	else
	{
		data_available = false;
	}

	return data_available;
}

void Spartan2OEM::ProcessData()
{
	if (initialized)
	{
		if (data_available)
		{
			//uint8_t i2c_addr        = rx_buffer[0];
			version_hs              = rx_buffer[1];
			uint16_t pump_current16 = (rx_buffer[2] << 8) + rx_buffer[3];
			uint8_t ri              = rx_buffer[4];
			//uint8_t status8         = rx_buffer[5];

			lambda1000    = LookupTables::GetLambda1000(pump_current16);
			temperature_c = LookupTables::GetTemperatureC(ri);
		}
	}
}

float Spartan2OEM::Lambda() const
{
	return (float)lambda1000 / (float)1000;
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

//TODO:
bool Spartan2OEM::NewData() const
{
	return initialized;
}
