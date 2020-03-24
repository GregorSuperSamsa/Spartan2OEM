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
	void ProcessData();

	float Lambda() const;
	float AfrRatio() const;

	uint16_t TemperatureC() const;
	uint16_t TemperatureF() const;

	uint8_t Status() const;

	void SetTimeot(const uint16_t &millis);
	uint16_t Timeout() const;
	uint8_t Version() const;

private:
	static const uint8_t I2C_RESPONSE_LENGTH;
	static const uint8_t I2C_RW_CMD;
	static const uint8_t I2C_SLAVE_MIN_ADDRESS;
	static const uint8_t I2C_SLAVE_MAX_ADDRESS;

	uint8_t rx_buffer[10];
	uint8_t i2c_address;
	uint16_t timeout_ms;
	TwoWire *i2c_bus;
	uint16_t lambda;
	uint16_t temperature_c;
	bool data_available;
	uint8_t version_hs;

};

#endif
