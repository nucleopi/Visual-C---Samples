/****************************************************************************************************************/
/*                                                                                                              */
/*   Copyright (c) Bogdan Mihalcea 2017                                                                         */
/*                                                                                                              */
/****************************************************************************************************************/

#include "rth03.h"

inline sensor_data rht03::read()
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

	sensor_data ret_val;

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
		ret_val.data1 = databuf.humidity * 0.1;
		ret_val.data2 = databuf.temperature * 0.1;
		ret_val.result_state = std::make_error_code(static_cast<std::errc>(0));

	}
	else
	{
		ret_val.result_state = std::make_error_code(std::errc::protocol_error);
	}

	return ret_val;
}

std::error_code rht03::write(sensor_data & data)
{
	return std::make_error_code(std::errc::not_supported);
}

std::error_code rht03::sample()
{
	m_sample_data = read();
	return m_sample_data.result_state;
}

std::string rht03::to_string()
{
	char buff[64];
	snprintf(buff, sizeof(buff), "\n Hum:%.02f%% \t\t Temp:%.02fC\n", m_sample_data.data1, m_sample_data.data2);

	return buff;
}
