/****************************************************************************************************************/
/*                                                                                                              */
/*   Copyright (c) Bogdan Mihalcea 2017                                                                         */
/*                                                                                                              */
/****************************************************************************************************************/

#pragma once
#include "one_wire_base.h"

class rht03 : public one_wire_base
{
protected:
	virtual sensor_data read();
	virtual std::error_code write(sensor_data& data);

public:
	rht03(std::string name, uint32_t dopin) : one_wire_base(name, dopin, 0) {}
	
	virtual std::error_code sample();
	virtual std::string to_string();
};