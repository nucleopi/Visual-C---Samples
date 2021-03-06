/****************************************************************************************************************/
/*                                                                                                              */
/*   Copyright (c) Bogdan Mihalcea 2017                                                                         */
/*                                                                                                              */
/****************************************************************************************************************/

#include "ir_base.h"

std::error_code ir_base::sample()
{
	return std::error_code();
}

const sensor_data& ir_base::get_data()
{
	return sensor_data();
}

std::error_code ir_base::set_data(sensor_data& data)
{
	return std::error_code();
}

ir_base::ir_base(std::string name, uint32_t dopin, uint32_t dipin) : sensor_transmission_base(name, dopin, dipin) {}

