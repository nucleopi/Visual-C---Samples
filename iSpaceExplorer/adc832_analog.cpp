/****************************************************************************************************************/
/*                                                                                                              */
/*   Copyright (c) Bogdan Mihalcea 2017                                                                         */
/*                                                                                                              */
/****************************************************************************************************************/

#include "adc832_analog.h"

adc832_analog::adc832_analog(std::string name, uint32_t dopin, uint32_t dipin, uint32_t cspin, uint32_t clockpin, uint32_t channel) : adc832(name, dopin, dipin, cspin, clockpin, channel)
{
}

adc832_analog::~adc832_analog()
{
}

sensor_data adc832_analog::read()
{
	sensor_data data;
	data.result_state = std::make_error_code(static_cast<std::errc>(0));
	return data;
}

std::error_code adc832_analog::write(sensor_data & data) 
{ 
	return std::make_error_code(std::errc::not_supported); 
}
