#pragma once
#include "pch.h"
#include "sensor_base.h"

uint32_t constexpr HC_SR04_TIMEOUT = 500000;

class hc_sr04 : public sensor_base
{
	uint32_t m_echo = 0;
	uint32_t m_trigger = 0;
public:
	hc_sr04();
	hc_sr04(std::string name, uint32_t echo, uint32_t trigger);
	~hc_sr04();

	virtual std::error_code write(sensor_data & data);
	virtual sensor_data read();
	virtual std::shared_ptr<std::string> name() const;
};

