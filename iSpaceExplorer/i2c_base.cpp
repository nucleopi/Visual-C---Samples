/****************************************************************************************************************/
/*                                                                                                              */
/*   Copyright (c) Bogdan Mihalcea 2017                                                                         */
/*                                                                                                              */
/****************************************************************************************************************/

#include "i2c_base.h"

i2c_base::i2c_base(uint16_t id)
{
	m_file_handle = wiringPiI2CSetup(id);
}

i2c_base::~i2c_base()
{
}

sensor_data i2c_base::read()
{
	sensor_data data;
	data.i32data1 = wiringPiI2CRead(m_file_handle);
	data.result_state = std::make_error_code(static_cast<std::errc>(0));
	return data;
}

std::error_code i2c_base::write(sensor_data & data)
{
	return wiringPiI2CWrite(m_file_handle, data.i32data1) == 0 ? ERROR_SUCCESS : std::make_error_code(std::errc::io_error);
}

int i2c_base::write_reg8(int reg, int data)
{
	return wiringPiI2CWriteReg8(m_file_handle, reg, data);
}

int i2c_base::write_reg16(int reg, int data)
{
	return wiringPiI2CWriteReg16(m_file_handle, reg, data);
}

int i2c_base::read_reg8(int reg)
{
	return wiringPiI2CReadReg8(m_file_handle, reg);
}

int i2c_base::read_reg16(int reg)
{
	return wiringPiI2CReadReg8(m_file_handle, reg);
}
