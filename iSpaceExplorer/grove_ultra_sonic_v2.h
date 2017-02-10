#pragma once

#include "pch.h"
#include "sensor_base.h"

uint32_t constexpr GROVE_ULTRA_SONIC_V2_TIMEOUT = 500000;

class grove_ultra_sonic_v2: public sensor_base
{
	uint32_t m_echo;

public:
	grove_ultra_sonic_v2();
	grove_ultra_sonic_v2(std::string name, uint32_t pin);
	~grove_ultra_sonic_v2();
	virtual sensor_data read();
	virtual std::error_code write(sensor_data& data);
	virtual std::shared_ptr<std::string> name() const;
};

