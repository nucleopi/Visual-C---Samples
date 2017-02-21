/****************************************************************************************************************/
/*                                                                                                              */
/*   Copyright (c) Bogdan Mihalcea 2017                                                                         */
/*                                                                                                              */
/****************************************************************************************************************/

#include "dht11.h"

 sensor_data dht11::read()
{
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
		 uint8_t humidity_int;
		 uint8_t humidity_remainder;
		 uint8_t temperature_int;
		 uint8_t temperature_remainder;
		 uint8_t crc;
	 } databuf = {};
	
#pragma pack(pop)

	sensor_data ret_val;

	//initiate handshake
	begin_receive();

	receive_data(&databuf, &databuf.humidity_int);
	receive_data(&databuf, &databuf.humidity_remainder);
	receive_data(&databuf, &databuf.temperature_int);
	receive_data(&databuf, &databuf.temperature_remainder);
	receive_data(&databuf, &databuf.crc);

	//conclude receive and reset state so we are ready to receive again
	end_receive();

	bool result = check_crc(reinterpret_cast<const uint8_t*>(&(databuf.humidity_int)), sizeof(uint8_t)*4, databuf.crc);

	if (result)
	{
		ret_val.data1 = create_float(databuf.humidity_int, databuf.humidity_remainder);
		ret_val.data2 = create_float(databuf.temperature_int, databuf.temperature_remainder);
		ret_val.result_state = std::make_error_code(static_cast<std::errc>(0));
	}
	else
	{
		ret_val.result_state = std::make_error_code(std::errc::protocol_error);
	}

	return ret_val;

}

 std::error_code dht11::write(sensor_data & data)
 {
	 return std::make_error_code(std::errc::not_supported);
 }

 std::error_code dht11::sample()
 {
	 m_sample_data = read();
	 return m_sample_data.result_state;
 }

 std::string dht11::to_string()
 {
	 char buff[64];
	 snprintf(buff, sizeof(buff), "\n Hum:%.f%% \t\t Temp:%.fC\n", m_sample_data.data1, m_sample_data.data2);
	 
	 return buff;
 }
