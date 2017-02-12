/****************************************************************************************************************/
/*                                                                                                              */ 
/*                                                                                                              */
/*   Bogdan Mihalcea                                                                                            */ 
/*   bogdanmihalcea@yahoo.com                                                                                   */
/*                                                                                                              */  
/****************************************************************************************************************/

#include "pch.h"
#include "hc_sr04.h"

hc_sr04::hc_sr04()
{

}

hc_sr04::hc_sr04(std::string name, uint32_t echo, uint32_t trigger)
{
	m_name = std::make_shared<std::string>(name);
	m_echo = echo;
	m_trigger = trigger;

	pinMode(m_echo, INPUT);
	pinMode(m_trigger, OUTPUT);
	delay(200);
}

hc_sr04::~hc_sr04()
{

}

std::error_code hc_sr04::write(sensor_data & data) { return std::make_error_code(std::errc::not_supported); }

sensor_data hc_sr04::read()
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
			sensor_data err;
			err.data1 = 0;
			err.result_state = std::make_error_code(std::errc::timed_out);
			return err;
		}
	}

	uint32_t start_echo_measure = micros();
	state_start_time = start_echo_measure;

	while (digitalRead(m_echo) == HIGH)
	{
		if (micros() - state_start_time > HC_SR04_TIMEOUT)
		{
			sensor_data err;
			err.data1 = 0;
			err.result_state = std::make_error_code(std::errc::timed_out);
			return err;
		}
	}
	
	sensor_data data;
	data.data1 = (micros() - start_echo_measure) * 343.0 / 2000;
	data.result_state = std::make_error_code(static_cast<std::errc>(0));

	return data;
}

std::shared_ptr<std::string> hc_sr04::name() const
{
	return std::shared_ptr<std::string>(new std::string("hc_sr04"));
}

