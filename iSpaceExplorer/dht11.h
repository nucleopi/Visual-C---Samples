/****************************************************************************************************************/
/*                                                                                                              */
/*   Copyright (c) Bogdan Mihalcea 2017                                                                         */
/*                                                                                                              */
/****************************************************************************************************************/

#pragma once

#include "one_wire_base.h"

inline float create_float(uint8_t int_val, uint8_t remainder_val)
{
	int precision = 10;
	if (remainder_val > 99) precision = 1000;
	else if (remainder_val > 9) precision = 100;

	float result = (float)(int_val * 1.0 + remainder_val / precision);

	return result;
}

class dht11 : public one_wire_base
{

protected:
	virtual sensor_data read();
	virtual std::error_code write(sensor_data& data);
public:
	dht11(std::string name, uint32_t dopin) : one_wire_base(name, dopin, 0) {}

	virtual std::error_code sample();
	virtual std::string to_string();
};