/****************************************************************************************************************/
/*                                                                                                              */
/*   Copyright (c) Bogdan Mihalcea 2017                                                                         */
/*                                                                                                              */
/****************************************************************************************************************/

#pragma once
#include "one_wire_base.h"

class rht03_data : public sensor_data
{
protected:
	float m_temperature;
	float m_humidity;

public:
	rht03_data() : m_temperature(0.0), m_humidity(0.0) {}
	rht03_data(rht03_data& data);

	virtual void set_data(float temp, float hum, std::error_code state);
	virtual const float get_temperature();
	virtual const float get_humidity();
	virtual const std::string to_string();
};

class rht03 : public one_wire_base
{
	rht03_data m_sample_data;

public:
	rht03(std::string name, uint32_t dopin);
	
	virtual std::error_code sample();
	virtual const sensor_data& get_data();
	virtual std::error_code set_data(const sensor_data& data);
};