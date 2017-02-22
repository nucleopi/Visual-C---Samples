/****************************************************************************************************************/
/*                                                                                                              */
/*   Copyright (c) Bogdan Mihalcea 2017                                                                         */
/*                                                                                                              */
/****************************************************************************************************************/

#include "pch.h"
#include "hc_sr04.h"

hc_sr04::hc_sr04()
{

}

hc_sr04::hc_sr04(std::string name, uint32_t echo, uint32_t trigger)
{
	m_name = name;
	m_echo = echo;
	m_trigger = trigger;

	pinMode(m_echo, INPUT);
	pinMode(m_trigger, OUTPUT);
	delay(200);
}

hc_sr04::~hc_sr04()
{

}

const sensor_data& hc_sr04::get_data()
{
	return m_sample_data;
}

std::error_code hc_sr04::set_data(const sensor_data& data)
{ 
	return std::make_error_code(std::errc::not_supported); 
}

std::error_code hc_sr04::sample()
{

	delay(10);

	digitalWrite(m_trigger, HIGH);
	delayMicroseconds(20);
	digitalWrite(m_trigger, LOW);

	uint32_t state_start_time = micros();

	while (digitalRead(m_echo) == LOW)
	{
		if (micros() - state_start_time > HC_SR04_TIMEOUT)
		{
			m_sample_data.set_data(0.0, std::make_error_code(std::errc::timed_out));
			return m_sample_data.error_code();
		}
	}

	uint32_t start_echo_measure = micros();
	state_start_time = start_echo_measure;

	while (digitalRead(m_echo) == HIGH)
	{
		if (micros() - state_start_time > HC_SR04_TIMEOUT)
		{
			m_sample_data.set_data(0.0, std::make_error_code(std::errc::timed_out));
			return m_sample_data.error_code();
		}
	}
	
	m_sample_data.set_data((float)((micros() - start_echo_measure) * 343.0 / 2000), ERROR_SUCCESS);

	return m_sample_data.error_code();
}

hc_sr04_data::hc_sr04_data(hc_sr04_data & data) : sensor_data(data)
{
	m_distance = data.m_distance;
}

void hc_sr04_data::set_data(float dist, std::error_code state)
{
	m_distance = dist;
	m_result_state = state;
}

const float hc_sr04_data::get_distance()
{
	return m_distance;
}


const std::string hc_sr04_data::to_string()
{
	char buff[64];
	snprintf(buff, sizeof(buff), "\n Dist:%.02f%%cm \n", m_distance);
	return buff;
}
