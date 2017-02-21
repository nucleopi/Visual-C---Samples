/****************************************************************************************************************/
/*                                                                                                              */
/*   Copyright (c) Bogdan Mihalcea 2017                                                                         */
/*                                                                                                              */
/****************************************************************************************************************/

#pragma once

#include "ir_base.h"

class ir_generic :
	public ir_base
{
protected:
	virtual sensor_data read();
	virtual std::error_code write(sensor_data& data);
public:
	ir_generic(std::string name, uint32_t dopin, uint32_t dipin);
	~ir_generic();

	virtual std::error_code sample();
	virtual std::string to_string();
	virtual std::error_code send(sensor_data& data);
};

