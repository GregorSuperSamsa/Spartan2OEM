#ifndef _LOOKUP_TABLES_H_
#define _LOOKUP_TABLES_H_

#include <Arduino.h>


class LookupTables
{

public:
	static uint16_t GetLambda1000(const uint16_t &pump_current);
	static uint16_t GetTemperatureC(const uint8_t &ri);

private:
	// Lookup table to convert LSU pump current to lambda
	// static const uint16_t LSU_PUMP_CURRENT_TO_LAMBDA_OFFSET;
	static const uint16_t LSU_PUMP_CURRENT_TO_LAMBDA_INDEX_LAST;
	static const uint16_t LSU_PUMP_CURRENT_TO_LAMBDA[];

	//Lookup table to convert Ri (nernst resistance) to sensor temperature[C]
	static const uint8_t RI_TO_TEMP_C_OFFSET;
	static const uint8_t RI_TO_TEMP_C_INDEX_LAST;
	static const uint16_t RI_TO_TEMP_C[];
};

#endif
