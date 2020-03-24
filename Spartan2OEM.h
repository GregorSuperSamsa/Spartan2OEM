#ifndef _SPARTAN2OEM_H_
#define _SPARTAN2OEM_H_

#include <Arduino.h>
#include <Wire.h>


class Spartan2OEM
{
public:
	Spartan2OEM(TwoWire* i2c_bus, const uint8_t &i2c_address = I2C_SLAVE_MIN_ADDRESS);

	bool ChangeI2CAddress(const uint8_t &new_address);
	bool RequestData();
	bool ProcessData();

	float Lambda() const;
	float AfrRatio() const;

	uint16_t TemperatureC() const;
	uint16_t TemperatureF() const;

	uint8_t Status() const;

	void SetTimeot(const uint16_t &millis);
	uint16_t Timeout() const;

private:
	static const uint8_t I2C_RESPONSE_LENGTH   = 6;
	static const uint8_t I2C_RW_CMD            = 0;
	static const uint8_t I2C_SLAVE_MIN_ADDRESS = 1;
	static const uint8_t I2C_SLAVE_MAX_ADDRESS = 16;

	uint8_t rx_buffer[10];
	uint8_t i2c_address;
	uint16_t timeoutMs;
	TwoWire *i2c_bus;

};

#endif
