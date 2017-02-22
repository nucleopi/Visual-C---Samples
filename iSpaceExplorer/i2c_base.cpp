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

std::error_code i2c_base::sample()
{
	m_sample_data.set_data(wiringPiI2CRead(m_file_handle), ERROR_SUCCESS);
	return m_sample_data.error_code();
}

const sensor_data& i2c_base::get_data()
{
	return m_sample_data;
}

std::error_code i2c_base::set_data(sensor_data & data)
{
	sensor_basic_data d = dynamic_cast<sensor_basic_data&>(data); 
	return wiringPiI2CWrite(m_file_handle, d.get_u32data()) == 0 ? ERROR_SUCCESS : std::make_error_code(std::errc::io_error);
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
