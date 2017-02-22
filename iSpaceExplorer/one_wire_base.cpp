/****************************************************************************************************************/
/*                                                                                                              */
/*   Copyright (c) Bogdan Mihalcea 2017                                                                         */
/*                                                                                                              */
/****************************************************************************************************************/

#include "one_wire_base.h"

static constexpr int HIGH_TIME = 32;
static constexpr int SENSOR_ANSWER_PERIOD_MAX_US = 100;
static constexpr int LOGIC_HIGH_PERIOD_MAX_US = 100;
static constexpr int RAISING_EDGE_PERIOD_US = 60;
static constexpr int LOGICAL_LOW_HIGH_BOUNDARY = 35;

one_wire_base::one_wire_base(std::string name, uint32_t dopin): one_wire_base(name, dopin, 0)
{
	
}

one_wire_base::one_wire_base(std::string name, uint32_t dopin, uint32_t dipin) : sensor_transmission_base(name, dopin, dipin)
{
}

void one_wire_base::begin_receive()
{
	uint64_t spin = 0;
	reset_time(100);

	pinMode(m_output_pin, OUTPUT); // set mode to output
	record_time();

	// Host pulls down and up the pins 
	digitalWrite(m_output_pin, 0);
	delay(18);
	digitalWrite(m_output_pin, 1);
	delayMicroseconds(20);

	//Allow sensor to answer
	pinMode(m_output_pin, INPUT); // set mode to input

	//Wait for the sensor to pull down as that signals the begining of the handshake answer
	wait_on_signal_transition_to(signal_logical_value::high, spin, 10 * SENSOR_ANSWER_PERIOD_MAX_US);

	//Sensor handshake response with up/down 80/80us
	wait_on_signal_transition_to(signal_logical_value::low, spin, SENSOR_ANSWER_PERIOD_MAX_US);
	wait_on_signal_transition_to(signal_logical_value::high, spin, SENSOR_ANSWER_PERIOD_MAX_US);
}

void one_wire_base::end_receive()
{
	pinMode(m_output_pin, OUTPUT); // set mode to output
	digitalWrite(m_output_pin, 1); // output a high level 
}

bool one_wire_base::check_crc(const uint8_t * databuf, size_t count, uint8_t crc)
{
	uint8_t datasum_8bit = 0;
	for (size_t i = 0; i < count; i++)
	{
		datasum_8bit += databuf[i];
	}

	if (debug_mode)
	{
		printf("CRC: %x, computed CRC : %x\n",
			(uint8_t)crc, uint8_t(datasum_8bit));

		uint32_t samples = times.size();
		std::chrono::high_resolution_clock::time_point prev;

		for (uint32_t t = 0; t < samples - 1; t = t + 2)
		{
			uint64_t time1 = std::chrono::duration_cast<std::chrono::microseconds>(times[t] - prev).count();
			uint64_t time2 = std::chrono::duration_cast<std::chrono::microseconds>(times[t + 1] - times[t]).count();
			prev = times[t + 1];
			printf("%d, %d\n", time1, time2);
		}
		printf("0x0\n");
	}


	return crc == datasum_8bit;
}
