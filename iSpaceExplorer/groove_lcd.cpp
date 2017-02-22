/****************************************************************************************************************/
/*                                                                                                              */
/*   Copyright (c) Bogdan Mihalcea 2017                                                                         */
/*                                                                                                              */
/****************************************************************************************************************/

#include "groove_lcd.h"

groove_lcd::groove_lcd(uint16_t id): i2c_base(id)
{
}

groove_lcd::~groove_lcd()
{
}

std::error_code groove_lcd::sample()
{
	return std::make_error_code(std::errc::not_supported);
}

const sensor_data& groove_lcd::get_data()
{
	return sensor_data();
}

std::error_code groove_lcd::set_data(const sensor_data& data)
{
	return std::make_error_code(std::errc::not_supported);
}

void groove_lcd::print(const std::string &str)
{
	sensor_basic_data data;
	data.set_data(LCD_ADDRESS, ERROR_SUCCESS);
	set_data(data);

	for (auto c : str)
	{
		data.set_data(c, ERROR_SUCCESS);
		set_data(data);
	}
}
