#pragma once

#include "pch.h"

#include <stdio.h>
#include <cstdint>
#include <wiringPi.h>
#include <vector>
#include <stdexcept>
#include <chrono>
#include <system_error>

#define ERROR_SUCCESS std::make_error_code(static_cast<std::errc>(0))

struct sensor_data
{
	std::error_code result_state;
	double data1 = 0.0;
	double data2 = 0.0;
	int32_t i32data1 = 0;
	int32_t i32data2 = 0;
	int16_t i16data1 = 0;
};

class sensor_base
{
protected:
	bool debug_mode = false;
	static bool is_inited;
	std::shared_ptr<std::string> m_name;
public:
	sensor_base();
	~sensor_base();

	std::error_code init();
	void set_debug_mode(bool mode);
	virtual sensor_data read() = 0;
	virtual std::error_code write(sensor_data& data) = 0;
	virtual std::shared_ptr<std::string> name() const;
};

