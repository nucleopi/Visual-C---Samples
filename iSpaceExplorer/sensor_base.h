/****************************************************************************************************************/
/*                                                                                                              */
/*   Copyright (c) Bogdan Mihalcea 2017                                                                         */
/*                                                                                                              */
/****************************************************************************************************************/

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
	std::string m_name;
	sensor_data m_sample_data;

	std::error_code init();
	virtual sensor_data read() = 0;
	virtual std::error_code write(sensor_data& data) = 0;

public:
	sensor_base();
	~sensor_base();

	void set_debug_mode(bool mode);
	virtual std::error_code sample() = 0;
	virtual std::string to_string() = 0;
	virtual std::string name() const;
};

