/****************************************************************************************************************/
/*                                                                                                              */
/*   Copyright (c) Bogdan Mihalcea 2017                                                                         */
/*                                                                                                              */
/****************************************************************************************************************/

#pragma once

#include "pch.h"

#include "sensor_base.h"
#include <wiringPiI2C.h>

class i2c_base : public sensor_base
{
	uint32_t m_file_handle;

protected:
	virtual sensor_data read();
	virtual std::error_code write(sensor_data & data);

	int write_reg8(int reg, int data);
	int write_reg16(int reg, int data);
	int read_reg8(int reg);
	int read_reg16(int reg);

public:
	i2c_base(uint16_t id);
	~i2c_base();
};

