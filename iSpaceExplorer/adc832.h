#pragma once

#include "pch.h"
#include "sensor_base.h"

class adc832: public sensor_base
{
	uint32_t m_dipin;
	uint32_t m_dopin;
	uint32_t m_cspin;
	uint32_t m_clockpin;
	uint32_t m_channel;

public:
	adc832();
	adc832(std::string name, uint32_t dopin, uint32_t dipin, uint32_t cspin, uint32_t clockpin, uint32_t channel);
	~adc832();
	uint32_t read_adc();
	virtual std::shared_ptr<std::string> name() const;
};

