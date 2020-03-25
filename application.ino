#include "Arduino.h"
#include "Wire.h"
#include "Spartan2OEM.h"


#define SPARTAN_2_OEM_COUNT 16


Spartan2OEM spartan2oem[SPARTAN_2_OEM_COUNT];

enum STATES {STATE_REQUEST, STATE_PROCESS, STATE_SEND};
uint8_t state = STATE_REQUEST;

void setup()
{
	// Initialize serial interface
	Serial.begin(9600);

	// Initialize I2C interface
	Wire.begin();

	// Initialize Spartan2OEM instances
	for (int i = 0; i < SPARTAN_2_OEM_COUNT; ++i)
	{
		spartan2oem[i].Initialize(&Wire, i + 1);
	}
}

void loop()
{
	switch(state)
	{
	case STATE_REQUEST:
		for (int i = 0; i < SPARTAN_2_OEM_COUNT; ++i)
		{
			spartan2oem[i].RequestData();
		}
		state = STATE_PROCESS;
		break;
	case STATE_PROCESS:
		for (int i = 0; i < SPARTAN_2_OEM_COUNT; ++i)
		{
			spartan2oem[i].ProcessData();
		}
		state = STATE_SEND;
		break;
	case STATE_SEND:
		for (int i = 0; i < SPARTAN_2_OEM_COUNT; ++i)
		{
			if (spartan2oem[i].NewData())
			{
				Serial.print(i);
				Serial.println(". Spartan 2 OEM");
				//
				Serial.print("Lambda: ");
				Serial.println(spartan2oem[i].Lambda());
				//
				Serial.print("Temperature [C]: ");
				Serial.println(spartan2oem[i].TemperatureC());
			}
		}
		state = STATE_REQUEST;
		break;
	default:
		break;
	}
}
