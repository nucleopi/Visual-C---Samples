/****************************************************************************************************************/
/*                                                                                                              */
/*   Copyright (c) Bogdan Mihalcea 2017                                                                         */
/*                                                                                                              */
/****************************************************************************************************************/

#pragma once
#include "pch.h"
#include "sensor_base.h"

uint32_t constexpr HC_SR04_TIMEOUT = 500000;

class hc_sr04_data : public sensor_data
{
protected:
	float m_distance;

public:
	hc_sr04_data() : m_distance(0.0) {}
	hc_sr04_data(hc_sr04_data& data);

	virtual void set_data(float dist, std::error_code state);
	virtual const float get_distance();
	virtual const std::string to_string();
};

class hc_sr04 : public sensor_base
{
	uint32_t m_echo = 0;
	uint32_t m_trigger = 0;

	hc_sr04_data m_sample_data;

public:
	hc_sr04();
	hc_sr04(std::string name, uint32_t echo, uint32_t trigger);
	~hc_sr04();

	virtual std::error_code sample();
	virtual const sensor_data& get_data();
	virtual std::error_code set_data(const sensor_data& data);
};

