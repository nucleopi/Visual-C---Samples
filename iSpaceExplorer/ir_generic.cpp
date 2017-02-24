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


	//initiate handshake
	begin_receive(&databuf);

	receive_data(&databuf, &databuf.data1);
	receive_data(&databuf, &databuf.data2);


	//conclude receive and reset state so we are ready to receive again
	end_receive(&databuf);

	if (verbosity_mode)
	{
		printf("Sensor data raw: %x\n", databuf.data1);
		printf("Sensor data raw: %x\n", databuf.data2);
	}

	m_sample_data.set_data(databuf.data1, databuf.data2, ERROR_SUCCESS);

	return m_sample_data.error_code();
}

const sensor_data& ir_generic::get_data()
{
	return m_sample_data;
}

std::error_code ir_generic::set_data(const sensor_data& data)
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
	
	ir_generic_data d = dynamic_cast<ir_generic_data& >(const_cast<sensor_data&>(data));

	databuf.data1 = d.get_type();
	databuf.data2 = d.get_code();

	//initiate handshake
	begin_send(&databuf);

	send_data(&databuf, &databuf.data1);
	send_data(&databuf, &databuf.data2);

	//conclude receive and reset state so we are ready to receive again
	end_send(&databuf);

	if (verbosity_mode)
	{
		printf("Sensor data raw: %x\n", databuf.data1);
		printf("Sensor data raw: %x\n", databuf.data2);
	}

	return ERROR_SUCCESS;
}

ir_generic_data::ir_generic_data(ir_generic_data & data)
{
}

void ir_generic_data::set_data(uint32_t type, uint16_t code, std::error_code state)
{
	m_type = type;
	m_code = code;
	m_result_state = state;
}

const uint32_t ir_generic_data::get_type()
{
	return m_type;
}

const uint16_t ir_generic_data::get_code()
{
	return m_code;
}

const std::string ir_generic_data::to_string()
{
	char buff[64];
	snprintf(buff, sizeof(buff), "\n Type::%x \t\t Code:%x\n", m_type, m_code);
	return buff;
}
