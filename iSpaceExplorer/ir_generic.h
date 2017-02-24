/****************************************************************************************************************/
/*                                                                                                              */
/*   Copyright (c) Bogdan Mihalcea 2017                                                                         */
/*                                                                                                              */
/****************************************************************************************************************/

#pragma once

#include "ir_base.h"

class ir_generic_data : public sensor_data
{
protected:
	uint32_t m_type;
	uint16_t m_code;

public:
	ir_generic_data() : m_type(0), m_code(0) {}
	ir_generic_data(ir_generic_data& data);

	virtual void set_data(uint32_t type, uint16_t code, std::error_code state);
	virtual const uint32_t get_type();
	virtual const uint16_t get_code();
	virtual const std::string to_string();
};

class ir_generic : public ir_base
{
	ir_generic_data m_sample_data;
public:
	ir_generic(std::string name, uint32_t dopin, uint32_t dipin);
	~ir_generic();

	virtual std::error_code sample();
	virtual const sensor_data& get_data();
	virtual std::error_code set_data(const sensor_data& data);
};

