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

std::string groove_lcd::to_string()
{
	return std::string();
}

void groove_lcd::print(const std::string &str)
{
	sensor_data data;
	data.i32data1 = LCD_ADDRESS;
	write(data);

	for (auto c : str)
	{
		data.i32data1 = c;
		write(data);
	}
}
