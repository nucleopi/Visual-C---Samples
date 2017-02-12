/****************************************************************************************************************/
/*                                                                                                              */ 
/*                                                                                                              */
/*   Bogdan Mihalcea                                                                                            */ 
/*   bogdanmihalcea@yahoo.com                                                                                   */
/*                                                                                                              */  
/****************************************************************************************************************/

#pragma once
#include "sensor_base.h"
#include <vector>
#include <chrono>

enum class signal_logical_value
{
	high = 1,
	low = 0
};

class sensor_transmission_base :
	public sensor_base
{
protected:
	uint32_t m_output_pin;
	uint32_t m_input_pin;
	std::vector<std::chrono::high_resolution_clock::time_point> times;

	void reset_time(size_t estimated_samples);

	void record_time();

	sensor_transmission_base(std::string name, uint32_t dopin, uint32_t dipin);

	std::error_code wait_on_signal_transition_to(signal_logical_value lv, uint64_t& spin, uint64_t timeout);
	std::error_code set_signal_transition_to(signal_logical_value lv, uint64_t duration);

	template <typename T, typename Y>
	std::error_code receive_data(T* rawdata, Y* data, size_t bit_count = sizeof(Y) * 8)
	{
		if (!data || bit_count > sizeof(T) * 8)
			return std::make_error_code(std::errc::invalid_argument);

		uint64_t arr[64];
		int index = 0;

		uint64_t spin = 0;
		for (int i = 0; i < bit_count; i++)
		{
			//wait on rising edge
			wait_on_signal_transition_to(signal_logical_value::high, spin, rawdata->RAISING_EDGE_PERIOD_US);
			arr[index++] = spin;
			//receive bit value
			wait_on_signal_transition_to(signal_logical_value::low, spin, rawdata->LOGIC_HIGH_PERIOD_MAX_US);
			arr[index++] = spin;
			data[i / (sizeof(T) * 8)] <<= 1;

			//compute bit value
			if (spin > rawdata->LOGICAL_LOW_HIGH_BOUNDARY)
			{
				data[i / (sizeof(T) * 8)] |= 1;
			}

			if (debug_mode && spin >= rawdata->LOGIC_HIGH_PERIOD_MAX_US)
			{
				printf("bad sig LOGIC_HIGH_PERIOD_MAX_US, expected: %d actual %d for data_bit:%d\n", rawdata->LOGIC_HIGH_PERIOD_MAX_US, spin, i);
			}
		}

		return ERROR_SUCCESS;
	}

	template <typename T, typename Y>
	std::error_code send_data(T* rawdata, Y* data, size_t bit_count = sizeof(Y) * 8)
	{
		if (!data || bit_count > sizeof(T) * 8)
			return std::make_error_code(std::errc::invalid_argument);

		Y mask;

		for (int i = 0; i < bit_count; i++)
		{
			mask = 1;
			mask <<= bit_count - 1 - i;
			set_signal_transition_to(signal_logical_value::low, rawdata->RAISING_EDGE_PERIOD_US);
			set_signal_transition_to(signal_logical_value::high, ((*data & mask) > 0) ? rawdata->LOGIC_HIGH_PERIOD_MAX_US : rawdata->LOGIC_LOW_PERIOD_MAX_US);
			
		}

		return ERROR_SUCCESS;
	}

};

