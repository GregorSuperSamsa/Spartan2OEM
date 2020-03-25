/***************************************************************************************************/
/*
   An Arduino library for the Spartan2 OEM I2C wideband controller.
   Based on the examples provided by Alan To (www.14point7.com). The developer of the Spartan2 OEM.

   Communication protocol is I2C.

   Connect Arduino SDA to Spartam2 OEM SDA
   Connect Arduino SLC to Spartam2 OEM SCL
   Both SDA and SLC lines require 4.7k pull-up resistors.

   For additional information, please, refer to the Spartan2 OEM I2C manual

   written by:  Gregor Samsa
   email:       georgi8georgiev@gmail.com
   source code: github.com/GregorSuperSamsa/Spartan2OEM
 */
/***************************************************************************************************/

#ifndef _SPARTAN2OEM_H_
#define _SPARTAN2OEM_H_

#include <Arduino.h>
#include <Wire.h>


class Spartan2OEM
{
public:
	Spartan2OEM();

	// Setup device
	void Initialize(TwoWire* i2c_bus, const uint8_t &i2c_address);

	// Please, read the provided Spartan2 OEM documentation
	bool ChangeI2CAddress(const uint8_t &new_address);

	// Request i2c data from the Spartan2OEM device
	bool RequestData();

	// Process read data
	void ProcessData();

	bool NewData() const;

	// Get sensor reading in Lambda or AFR
	float Lambda() const;
	float AfrRatio() const;

	// Get sensor temperature in Celsius or Fahrenheit
	uint16_t TemperatureC() const;
	uint16_t TemperatureF() const;

	// Get Spartan2OEM hardware & software versions
	uint8_t Version() const;

	// Currently not used
	uint8_t Status() const;

	// Currently not used
	void SetTimeot(const uint16_t &millis);
	uint16_t Timeout() const;

private:
	static const uint8_t I2C_RESPONSE_LENGTH;
	static const uint8_t I2C_RW_CMD;
	static const uint8_t I2C_SLAVE_DEFAULT_ADDRESS;
	static const uint8_t I2C_SLAVE_MIN_ADDRESS;
	static const uint8_t I2C_SLAVE_MAX_ADDRESS;

	// Pointer to the i2c interface
	TwoWire *i2c_bus = nullptr;

	// Local buffer for the received i2c data
	uint8_t rx_buffer[10];

	// Current i2c address for the Spartan2OEM device
	uint8_t i2c_address = 0;

	// Current lambda reading * 1000
	uint16_t lambda1000 = 0;

	// Current temperature reading of the sensor [C]
	uint16_t temperature_c = 0;

	// Flag for new i2c data available
	bool data_available = false;

	// Flag for the Spartan2OEM object initialization
	bool initialized = false;

	// Current Spartan2OEM hardware & software versions
	uint8_t version_hs = 0;

	// Currently not used
	uint16_t timeout_ms = 0;
};

#endif
