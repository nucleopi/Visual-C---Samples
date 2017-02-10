#include "analog_sensor_adc832.h"

analog_sensor_adc832::analog_sensor_adc832(std::string name, uint32_t dopin, uint32_t dipin, uint32_t cspin, uint32_t clockpin, uint32_t channel) : adc832(name, dopin, dipin, cspin, clockpin, channel)
{
}

analog_sensor_adc832::~analog_sensor_adc832()
{
}

sensor_data analog_sensor_adc832::read()
{
	sensor_data data;
	data.result_state = std::make_error_code(static_cast<std::errc>(0));
	return data;
}

std::error_code analog_sensor_adc832::write(sensor_data & data) 
{ 
	return std::make_error_code(std::errc::not_supported); 
}
