/****************************************************************************************************************/
/*                                                                                                              */
/*   Copyright (c) Bogdan Mihalcea 2017                                                                         */
/*                                                                                                              */
/****************************************************************************************************************/

#include "dht11.h"

dht11::dht11(std::string name, uint32_t dopin) : one_wire_base(name, dopin) 
{
}

std::error_code dht11::sample()
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
		m_sample_data.set_data(create_float(databuf.temperature_int, databuf.temperature_remainder), create_float(databuf.humidity_int, databuf.humidity_remainder), ERROR_SUCCESS);
	}
	else
	{
		m_sample_data.set_data(0.0, 0.0, std::make_error_code(std::errc::protocol_error));
	}

	return m_sample_data.error_code();

}

 std::error_code dht11::set_data(const sensor_data & data)
 {
	 return std::make_error_code(std::errc::not_supported);
 }

 const sensor_data& dht11::get_data()
 {
	 return m_sample_data;
 }


 dht11_data::dht11_data(dht11_data & data) : sensor_data(data)
 {
	 m_temperature = data.m_temperature;
	 m_humidity = data.m_humidity;
 }

 void dht11_data::set_data(float temp, float hum, std::error_code state)
 {
	 m_temperature = temp;
	 m_humidity = hum;
	 m_result_state = state;
 }

 const float dht11_data::get_temperature()
 {
	 return m_temperature;
 }

 const float dht11_data::get_humidity()
 {
	 return m_humidity;
 }

 const std::string dht11_data::to_string()
 {
	 char buff[64];
	 snprintf(buff, sizeof(buff), "\n Hum:%.02f%% \t\t Temp:%.02fC\n", m_humidity, m_temperature);
	 return buff;
 }