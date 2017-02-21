#include "sensor_transmission_base.h"



void sensor_transmission_base::reset_time(size_t estimated_samples)
{
	times.clear();
	times.reserve(estimated_samples);
}

void sensor_transmission_base::record_time()
{
	times.push_back(std::chrono::high_resolution_clock::now());
}

sensor_transmission_base::sensor_transmission_base(std::string name, uint32_t dopin, uint32_t dipin) : m_output_pin(dopin), m_input_pin(dipin)
{
	std::error_code error = init();
	if (error)
		throw std::system_error(error);
}

std::error_code sensor_transmission_base::wait_on_signal_transition_to(signal_logical_value lv, uint64_t& spin, uint64_t timeout)
{
	spin = 0;

	auto start = std::chrono::high_resolution_clock::now();
	while (static_cast<signal_logical_value>(digitalRead(m_output_pin)) != lv && (spin < timeout || timeout == -1))
	{
		delayMicroseconds(1);
		spin = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start).count();
	}

	spin = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start).count();
	record_time();

	if (spin > timeout)
	{
		return std::make_error_code(std::errc::timed_out);
	}

	return ERROR_SUCCESS;
}

std::error_code sensor_transmission_base::set_signal_transition_to(signal_logical_value lv, uint64_t duration)
{
	digitalWrite(m_input_pin, static_cast<int>(lv));
	delayMicroseconds(duration);
	return ERROR_SUCCESS;
}

