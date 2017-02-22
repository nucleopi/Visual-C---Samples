/****************************************************************************************************************/
/*                                                                                                              */
/*   Copyright (c) Bogdan Mihalcea 2017                                                                         */
/*                                                                                                              */
/****************************************************************************************************************/


#include "grove_ultra_sonic_v2.h"

grove_ultra_sonic_v2::grove_ultra_sonic_v2()
{
}

grove_ultra_sonic_v2::grove_ultra_sonic_v2(std::string name, uint32_t pin)
{
	m_name = name;
	m_echo = pin;
	delay(200);
}


grove_ultra_sonic_v2::~grove_ultra_sonic_v2()
{
}

std::error_code grove_ultra_sonic_v2::sample()
{
	pinMode(m_echo, OUTPUT);
	delay(10);
	digitalWrite(m_echo, LOW);
	delayMicroseconds(2);
	digitalWrite(m_echo, HIGH);
	delayMicroseconds(5);
	digitalWrite(m_echo, LOW);

	pinMode(m_echo, INPUT);

	uint32_t state_start_time = micros();

	while (digitalRead(m_echo) == LOW)
	{
		if (micros() - state_start_time > GROVE_ULTRA_SONIC_V2_TIMEOUT)
		{
			m_sample_data.set_data(0.0, std::make_error_code(std::errc::timed_out));
			return m_sample_data.error_code();
		}
	}

	uint32_t start_echo_measure = micros();
	state_start_time = start_echo_measure;

	while (digitalRead(m_echo) == HIGH)
	{
		if (micros() - state_start_time > GROVE_ULTRA_SONIC_V2_TIMEOUT)
		{
			m_sample_data.set_data(0.0, std::make_error_code(std::errc::timed_out));
			return m_sample_data.error_code();
		}
	}

	m_sample_data.set_data((float)((micros() - start_echo_measure) * 343.0 / 2000), ERROR_SUCCESS);

	return m_sample_data.error_code();
}

std::error_code grove_ultra_sonic_v2::set_data(const sensor_data & data)
{ 
	return std::make_error_code(std::errc::not_supported); 
}

const sensor_data& grove_ultra_sonic_v2::get_data()
{
	return m_sample_data;
}


grove_ultra_sonic_v2_data::grove_ultra_sonic_v2_data(grove_ultra_sonic_v2_data & data) : sensor_data(data)
{
	m_distance = data.m_distance;
}

void grove_ultra_sonic_v2_data::set_data(float dist, std::error_code state)
{
	m_distance = dist;
	m_result_state = state;
}

const float grove_ultra_sonic_v2_data::get_distance()
{
	return m_distance;
}


const std::string grove_ultra_sonic_v2_data::to_string()
{
	char buff[64];
	snprintf(buff, sizeof(buff), "\n Dist:%.02f%%cm \n", m_distance);
	return buff;
}