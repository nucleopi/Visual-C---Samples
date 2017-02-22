/****************************************************************************************************************/
/*                                                                                                              */
/*   Copyright (c) Bogdan Mihalcea 2017                                                                         */
/*                                                                                                              */
/****************************************************************************************************************/

#pragma once

#include "sensor_base.h"
#include <wiringPiSPI.h>

class imu_data : public virtual sensor_data
{
protected:
	float m_x_acc;
	float m_y_acc;
	float m_z_acc;
	float m_acc;

public:
	imu_data() : m_x_acc(0.0), m_y_acc(0.0), m_z_acc(0), m_acc(0.0) {}
	imu_data(imu_data& data);

	virtual void set_data(float x_acc, float y_acc, float z_acc, float acc, std::error_code state);
	virtual const float get_x_acc();
	virtual const float get_y_acc();
	virtual const float get_z_acc();
	virtual const float get_acc();
	virtual const std::string to_string();
};

class imu_spi_base : public sensor_base
{
	uint8_t m_x_channel = 0;
	uint8_t m_y_channel = 0;
	uint8_t m_z_channel = 0;

	imu_data m_sample_data;

public:
	imu_spi_base();
	imu_spi_base(std::string name, uint8_t x_channel, uint8_t y_channel, uint8_t z_channel);
	~imu_spi_base();

	virtual std::error_code sample();
	virtual const sensor_data& get_data();
	virtual std::error_code set_data(sensor_data & data);
};