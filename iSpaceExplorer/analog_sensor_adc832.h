#pragma once
#include "adc832.h"

class analog_sensor_adc832 :
	public adc832
{
public:
	analog_sensor_adc832(std::string name, uint32_t dopin, uint32_t dipin, uint32_t cspin, uint32_t clockpin, uint32_t channel);
	~analog_sensor_adc832();
	virtual sensor_data read();
	std::error_code write(sensor_data & data);
};

