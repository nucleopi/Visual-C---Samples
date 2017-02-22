/****************************************************************************************************************/
/*                                                                                                              */
/*   Copyright (c) Bogdan Mihalcea 2017                                                                         */
/*                                                                                                              */
/****************************************************************************************************************/

#include "rth03.h"

rht03::rht03(std::string name, uint32_t dopin) : one_wire_base(name, dopin, 0) 
{
}

std::error_code rht03::sample()
{
	uint32_t crc = 0;

#pragma pack(push, 1)
	struct raw_data
	{
		size_t HIGH_TIME = 32;
		size_t SENSOR_ANSWER_HANDSHAKE_LOW_US = 100;
		size_t SENSOR_ANSWER_HANDSHAKE_HIGH_US = 120;
		size_t LOGIC_HIGH_PERIOD_MAX_US = 100;
		size_t RAISING_EDGE_PERIOD_US = 60;
		size_t LOGICAL_LOW_HIGH_BOUNDARY = 35;
		size_t LOGIC_LOW_PERIOD_MAX_US = 90;
		uint16_t humidity;
		uint16_t temperature;
		uint8_t crc;
	} databuf = {};
#pragma pack(pop)

	//initiate handshake
	begin_receive();

	receive_data(&databuf, &databuf.humidity);
	receive_data(&databuf, &databuf.temperature);
	receive_data(&databuf, &databuf.crc);

	//conclude receive and reset state so we are ready to receive again
	end_receive();

	if (debug_mode)
	{
		printf("Sensor humidity raw: %d\t temperature raw: %d\n", databuf.humidity, databuf.temperature);
	}

	bool result = check_crc(reinterpret_cast<const uint8_t*>(&(databuf.humidity)), sizeof(uint16_t) * 2, databuf.crc);

	if (result)
	{
		m_sample_data.set_data(databuf.temperature * 0.1, databuf.humidity * 0.1, ERROR_SUCCESS);
	}
	else
	{
		m_sample_data.set_data(0.0, 0.0, std::make_error_code(std::errc::protocol_error));
	}

	return m_sample_data.error_code();
}

const sensor_data&  rht03::get_data()
{
	return m_sample_data;
}

std::error_code rht03::set_data(const sensor_data & data)
{
	return std::make_error_code(std::errc::not_supported);
}

rht03_data::rht03_data(rht03_data & data) : sensor_data(data)
{
	m_temperature = data.m_temperature;
	m_humidity = data.m_humidity;
}

void rht03_data::set_data(float temp, float hum, std::error_code state)
{
	m_temperature = temp; 
	m_humidity = hum; 
	m_result_state = state; 
}

const float rht03_data::get_temperature()
{ 
	return m_temperature; 
}

const float rht03_data::get_humidity()
{ 
	return m_humidity; 
}

const std::string rht03_data::to_string()
{
	char buff[64];
	snprintf(buff, sizeof(buff), "\n Hum:%.02f%% \t\t Temp:%.02fC\n", m_humidity, m_temperature);
	return buff;
}
