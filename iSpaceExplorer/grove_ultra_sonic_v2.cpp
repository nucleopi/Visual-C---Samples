/****************************************************************************************************************/
/*                                                                                                              */ 
/*                                                                                                              */
/*   Bogdan Mihalcea                                                                                            */ 
/*   bogdanmihalcea@yahoo.com                                                                                   */
/*                                                                                                              */  
/****************************************************************************************************************/

#include "grove_ultra_sonic_v2.h"



grove_ultra_sonic_v2::grove_ultra_sonic_v2()
{
}

grove_ultra_sonic_v2::grove_ultra_sonic_v2(std::string name, uint32_t pin)
{
	m_name = std::make_shared<std::string>(name);
	m_echo = pin;
	delay(200);
}


grove_ultra_sonic_v2::~grove_ultra_sonic_v2()
{
}

sensor_data grove_ultra_sonic_v2::read()
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
		if (micros() - state_start_time > GROVE_ULTRA_SONIC_V2_TIMEOUT)
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

std::error_code grove_ultra_sonic_v2::write(sensor_data & data) { return std::make_error_code(std::errc::not_supported); }

std::shared_ptr<std::string> grove_ultra_sonic_v2::name() const
{
	return std::shared_ptr<std::string>(new std::string("grove_ultra_sonic_v2"));
}
