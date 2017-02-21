/****************************************************************************************************************/
/*                                                                                                              */
/*   Copyright (c) Bogdan Mihalcea 2017                                                                         */
/*                                                                                                              */
/****************************************************************************************************************/

#include "ir_generic.h"

ir_generic::ir_generic(std::string name, uint32_t dopin, uint32_t dipin): ir_base(name, dopin, dipin)
{
}

ir_generic::~ir_generic()
{
}

std::error_code ir_generic::sample()
{
	m_sample_data = read();
	return m_sample_data.result_state;
}

std::string ir_generic::to_string()
{
	std::string ret = std::to_string(m_sample_data.data1);
	ret.append(" ");
	ret.append(std::to_string(m_sample_data.data1));
	
	return ret;
}

std::error_code ir_generic::send(sensor_data & data)
{
	return write(data);
}

sensor_data ir_generic::read()
{
#pragma pack(push, 1)
	struct raw_data
	{
		size_t HIGH_TIME = 32;
		size_t SENSOR_ANSWER_HANDSHAKE_LOW_US = 3500;
		size_t SENSOR_ANSWER_HANDSHAKE_HIGH_US = 1700;
		size_t LOGIC_HIGH_PERIOD_MAX_US = 1250;
		size_t RAISING_EDGE_PERIOD_US = 650;
		size_t LOGICAL_LOW_HIGH_BOUNDARY = 600;
		size_t LOGIC_LOW_PERIOD_MAX_US = 500;
		uint32_t data1;
		uint16_t data2;
	} databuf = {};
#pragma pack(pop)

	sensor_data ret_val;

	//initiate handshake
	begin_receive(&databuf);

	receive_data(&databuf, &databuf.data1);
	receive_data(&databuf, &databuf.data2);


	//conclude receive and reset state so we are ready to receive again
	end_receive(&databuf);

	if (debug_mode)
	{
		printf("Sensor data raw: %x\n", databuf.data1);
		printf("Sensor data raw: %x\n", databuf.data2);
	}

	
	ret_val.i32data1 = databuf.data1;
	ret_val.i16data1 = databuf.data2;
	ret_val.result_state = std::make_error_code(static_cast<std::errc>(0));

	return ret_val;

}

std::error_code ir_generic::write(sensor_data& data)
{
#pragma pack(push, 1)
	struct raw_data
	{
		size_t HIGH_TIME = 7000;
		size_t SENSOR_ANSWER_HANDSHAKE_LOW_US = 3500;
		size_t SENSOR_ANSWER_HANDSHAKE_HIGH_US = 1700;
		size_t LOGIC_HIGH_PERIOD_MAX_US = 875;
		size_t RAISING_EDGE_PERIOD_US = 505;
		size_t LOGICAL_LOW_HIGH_BOUNDARY = 600;
		size_t LOGIC_LOW_PERIOD_MAX_US = 150;
		uint32_t data1;
		uint16_t data2;
	} databuf = {};
#pragma pack(pop)
	
	//initiate handshake
	begin_send(&databuf);

	send_data(&databuf, &data.i32data1);
	send_data(&databuf, &data.i16data1);

	//conclude receive and reset state so we are ready to receive again
	end_send(&databuf);

	if (debug_mode)
	{
		printf("Sensor data raw: %x\n", databuf.data1);
		printf("Sensor data raw: %x\n", databuf.data2);
	}

	return ERROR_SUCCESS;
}


