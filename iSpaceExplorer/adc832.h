#pragma once
/****************************************************************************************************************/
/*                                                                                                              */
/*   Copyright (c) Bogdan Mihalcea 2017                                                                         */
/*                                                                                                              */
/****************************************************************************************************************/

#pragma once
#include "pch.h"
#include "sensor_base.h"

class adc832: public virtual sensor_base
{
	uint32_t m_dipin;
	uint32_t m_dopin;
	uint32_t m_cspin;
	uint32_t m_clockpin;
	uint32_t m_channel;

	sensor_basic_data m_sample_data;

public:
	adc832();
	adc832(std::string name, uint32_t dopin, uint32_t dipin, uint32_t cspin, uint32_t clockpin, uint32_t channel);
	~adc832();
	virtual std::error_code sample();
	virtual const sensor_data& get_data();
	virtual std::error_code set_data(const sensor_data& data);
};

