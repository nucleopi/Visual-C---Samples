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

class sensor_data
{
protected:
	std::error_code m_result_state;

public:
	sensor_data() : m_result_state(ERROR_SUCCESS) {}
	sensor_data(sensor_data& data)
	{
		m_result_state = data.m_result_state;
	}
	virtual const std::error_code error_code() { return m_result_state; }
	virtual const std::string to_string() { return ""; }
};

class sensor_basic_data : public sensor_data
{
protected:
	uint32_t m_u32data;

public:
	sensor_basic_data() : m_u32data(0) { m_result_state = ERROR_SUCCESS; }
	sensor_basic_data(sensor_basic_data& data)
	{
		m_u32data = data.m_u32data;
		m_result_state = data.m_result_state;
	}
	virtual void set_data(uint32_t data, std::error_code state) { m_u32data = data; m_result_state = state; }
	virtual uint32_t get_u32data() { return m_u32data; }
	virtual const std::error_code error_code() { return m_result_state; }
	virtual const std::string to_string() { return std::to_string(m_u32data); }
};

class sensor_base
{
protected:
	bool verbosity_mode = false;
	static bool is_inited;
	std::string m_name;

	std::error_code init();

public:
	sensor_base();
	~sensor_base();

	void set_verbosity(bool mode);
	virtual std::error_code sample() = 0;
	virtual const std::string name();
	virtual const sensor_data& get_data() = 0;
	virtual std::error_code set_data(const sensor_data& data) = 0;
};

