/****************************************************************************************************************/
/*                                                                                                              */
/*   Copyright (c) Bogdan Mihalcea 2017                                                                         */
/*                                                                                                              */
/****************************************************************************************************************/

#pragma once

#include "sensor_transmission_base.h"
class ir_base :
	public sensor_transmission_base
{
public:
	ir_base(std::string name, uint32_t dopin, uint32_t dipin);
	
	template <typename T>
	void begin_receive(T* data);
	
	template <typename T>
	void end_receive(T* data);

	template<typename T>
	void begin_send(T * data);

	template<typename T>
	void end_send(T * data);
	
	template <typename T>
	bool check_crc(T* data);
};

template<typename T>
void ir_base::begin_receive(T * data)
{
	reset_time(100);

	//Allow sensor to answer
	pinMode(m_output_pin, INPUT); // set mode to input

	//Wait for the sensor to pull down as that signals the begining of the handshake answer
	uint64_t spin = 0;
	
	while (true)
	{
		wait_on_signal_transition_to(signal_logical_value::low, spin, -1);
		wait_on_signal_transition_to(signal_logical_value::high, spin, (uint64_t)(data->SENSOR_ANSWER_HANDSHAKE_LOW_US * 1.2));
		if (spin > data->SENSOR_ANSWER_HANDSHAKE_LOW_US * 0.8 && spin < (uint64_t)(data->SENSOR_ANSWER_HANDSHAKE_LOW_US * 1.2))
		{
			wait_on_signal_transition_to(signal_logical_value::low, spin, (uint64_t)(data->SENSOR_ANSWER_HANDSHAKE_HIGH_US * 1.2));
			if (spin > data->SENSOR_ANSWER_HANDSHAKE_HIGH_US * 0.8 && spin < (uint64_t)(data->SENSOR_ANSWER_HANDSHAKE_HIGH_US * 1.2))
			{
				record_time();
				break;
			}
		}
	}
}

template<typename T>
void ir_base::end_receive(T * data)
{

}

template<typename T>
void ir_base::begin_send(T * data)
{
	//Allow sensor to answer
	pinMode(m_input_pin, OUTPUT); // set mode to input

	//Send signals for the begining of the handshake
	set_signal_transition_to(signal_logical_value::high, data->HIGH_TIME);
	set_signal_transition_to(signal_logical_value::low, data->SENSOR_ANSWER_HANDSHAKE_LOW_US);
	set_signal_transition_to(signal_logical_value::high, data->SENSOR_ANSWER_HANDSHAKE_HIGH_US);
	set_signal_transition_to(signal_logical_value::low, data->RAISING_EDGE_PERIOD_US);
}

template<typename T>
void ir_base::end_send(T * data)
{
	set_signal_transition_to(signal_logical_value::high, 1);
}

template<typename T>
bool ir_base::check_crc(T * data)
{
	uint8_t datasum_8bit = 0;
	size_t count = sizeof(data->data);
	const uint8_t* data8bit = reinterpret_cast<const uint8_t*>(&(data->data));
	for (size_t i = 0; i < count; i++)
	{
		datasum_8bit += data8bit[i];
	}

	if (debug_mode)
	{
		printf("CRC: %x, computed CRC : %x\n",
			(uint8_t)data->crc, uint8_t(datasum_8bit));

		uint32_t samples = times.size();
		std::chrono::high_resolution_clock::time_point prev;

		for (int t = 0; t < samples - 1; t = t + 2)
		{
			uint32_t time1 = std::chrono::duration_cast<std::chrono::microseconds>(times[t] - prev).count();
			uint32_t time2 = std::chrono::duration_cast<std::chrono::microseconds>(times[t + 1] - times[t]).count();
			prev = times[t + 1];
			printf("%d, %d\n", time1, time2);
		}
		printf("0x0\n");
	}


	return data->crc == datasum_8bit;
}
