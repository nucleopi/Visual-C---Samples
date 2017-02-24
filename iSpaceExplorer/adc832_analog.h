/****************************************************************************************************************/
/*                                                                                                              */
/*   Copyright (c) Bogdan Mihalcea 2017                                                                         */
/*                                                                                                              */
/****************************************************************************************************************/

#pragma once
#include "adc832.h"

class adc832_analog : public adc832
{
	sensor_basic_data m_sample_data;
public:
	adc832_analog(std::string name, uint32_t dopin, uint32_t dipin, uint32_t cspin, uint32_t clockpin, uint32_t channel);
	~adc832_analog();
	virtual std::error_code sample();
	virtual const sensor_data& get_data();
	virtual std::error_code set_data(sensor_data& data);
};

