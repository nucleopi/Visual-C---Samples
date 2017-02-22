/****************************************************************************************************************/
/*                                                                                                              */
/*   Copyright (c) Bogdan Mihalcea 2017                                                                         */
/*                                                                                                              */
/****************************************************************************************************************/

#pragma once

#include "pch.h"
#include "sensor_base.h"

uint32_t constexpr GROVE_ULTRA_SONIC_V2_TIMEOUT = 500000;

class grove_ultra_sonic_v2_data : public virtual sensor_data
{
protected:
	float m_distance;

public:
	grove_ultra_sonic_v2_data() : m_distance(0.0) {}
	grove_ultra_sonic_v2_data(grove_ultra_sonic_v2_data& data);

	virtual void set_data(float dist, std::error_code state);
	virtual const float get_distance();
	virtual const std::string to_string();
};

class grove_ultra_sonic_v2: public sensor_base
{
	uint32_t m_echo;
	grove_ultra_sonic_v2_data m_sample_data;

public:
	grove_ultra_sonic_v2();
	grove_ultra_sonic_v2(std::string name, uint32_t pin);
	~grove_ultra_sonic_v2();

	virtual std::error_code sample();
	virtual const sensor_data& get_data();
	virtual std::error_code set_data(const sensor_data& data);
};

