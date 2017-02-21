/****************************************************************************************************************/
/*                                                                                                              */
/*   Copyright (c) Bogdan Mihalcea 2017                                                                         */
/*                                                                                                              */
/****************************************************************************************************************/

#pragma once
#include "adc832.h"

class adc832_analog :
	public adc832
{
public:
	adc832_analog(std::string name, uint32_t dopin, uint32_t dipin, uint32_t cspin, uint32_t clockpin, uint32_t channel);
	~adc832_analog();
	virtual sensor_data read();
	std::error_code write(sensor_data & data);
};

