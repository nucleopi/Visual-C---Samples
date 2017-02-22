/****************************************************************************************************************/
/*                                                                                                              */
/*   Copyright (c) Bogdan Mihalcea 2017                                                                         */
/*                                                                                                              */
/****************************************************************************************************************/

#include "imu_spi_base.h"
#include <math.h>

// read SPI data from adc (MCP3008 chip 8 possible adc's (0 thru 7))
uint32_t read_adc(uint8_t adc_channel, uint8_t max_adc_channels)
{
	uint8_t buff[3] = {};
	uint32_t adc_channel_data = 0;
	if ((adc_channel > max_adc_channels) || (adc_channel < 0))
		return -1;
	buff[0] = 1;
	buff[1] = (uint8_t)((8 + adc_channel) << 4);
	buff[2] = 0;
	wiringPiSPIDataRW(0, buff, 3);
	adc_channel_data = ((buff[1] & 3) << 8) + buff[2];
	return adc_channel_data;
}

std::error_code imu_spi_base::set_data(sensor_data& data)
{
	return std::error_code();
}

std::error_code imu_spi_base::sample()
{
	float acc_x = (float)(read_adc(m_x_channel, 8) - 513)/2;
	float acc_y = (float)(read_adc(m_y_channel, 8) - 513)/2;
	float acc_z = (float)(read_adc(m_z_channel, 8) - 511)/2;
	float acc = (float)sqrt(acc_x*acc_x + acc_y*acc_y + acc_z*acc_z);
	m_sample_data.set_data(acc_x, acc_y, acc_z, acc, ERROR_SUCCESS);

	return m_sample_data.error_code();
}

imu_spi_base::imu_spi_base() : sensor_base()
{
}

imu_spi_base::imu_spi_base(std::string name, uint8_t x_channel, uint8_t y_channel, uint8_t z_channel): m_x_channel(x_channel), m_y_channel(y_channel), m_z_channel (z_channel)
{
	m_name = name;
}

imu_spi_base::~imu_spi_base()
{
}

const sensor_data& imu_spi_base::get_data()
{
	return m_sample_data;
}

imu_data::imu_data(imu_data & data)
{
}

void imu_data::set_data(float x_acc, float y_acc, float z_acc, float acc, std::error_code state)
{
	m_x_acc = x_acc;
	m_y_acc = y_acc;
	m_z_acc = z_acc;
	m_acc = acc;
	m_result_state = state;
}

const float imu_data::get_x_acc()
{
	return m_x_acc;
}

const float imu_data::get_y_acc()
{
	return m_y_acc;
}

const float imu_data::get_z_acc()
{
	return m_z_acc;
}

const float imu_data::get_acc()
{
	return m_acc;
}

const std::string imu_data::to_string()
{
	char buff[64];
	snprintf(buff, sizeof(buff), "\n acc:%.02f%% [x:%.02f%% y:%.02f%% z:%.02f%%] \n", m_acc, m_x_acc, m_y_acc, m_z_acc);
	return buff;
}
