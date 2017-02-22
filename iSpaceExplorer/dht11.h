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


class dht11_data : public virtual sensor_data
{
protected:
	float m_temperature;
	float m_humidity;

public:
	dht11_data() : m_temperature(0.0), m_humidity(0.0) {}
	dht11_data(dht11_data& data);

	virtual void set_data(float temp, float hum, std::error_code state);
	virtual const float get_temperature();
	virtual const float get_humidity();
	virtual const std::string to_string();
};

class dht11 : public virtual one_wire_base
{
	dht11_data m_sample_data;
public:
	dht11(std::string name, uint32_t dopin);

	virtual std::error_code sample();
	virtual const sensor_data& get_data();
	virtual std::error_code set_data(const sensor_data& data);
};