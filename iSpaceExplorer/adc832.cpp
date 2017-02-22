/****************************************************************************************************************/
/*                                                                                                              */
/*   Copyright (c) Bogdan Mihalcea 2017                                                                         */
/*                                                                                                              */
/****************************************************************************************************************/

#include "adc832.h"

adc832::adc832()
{
}

adc832::adc832(std::string name, uint32_t dopin, uint32_t dipin, uint32_t cspin, uint32_t clockpin, uint32_t channel)
{
	m_name = name;
	m_dipin = dipin;
	m_dopin = dopin;
	m_cspin = cspin;
	m_clockpin = clockpin;
	m_channel = channel;

	delay(200);
}

adc832::~adc832()
{

}

std::error_code adc832::sample()
{
	pinMode(m_dipin, OUTPUT);
	pinMode(m_dopin, INPUT);
	pinMode(m_cspin, OUTPUT);
	pinMode(m_clockpin, OUTPUT);


	digitalWrite(m_cspin, HIGH); //clear last transmission
	digitalWrite(m_cspin, LOW);

	digitalWrite(m_clockpin, LOW); //start clock low

								   // using ADC 832 which has 2 channels
	for (uint8_t i = 0; i < 2; i++)
	{
		if (m_channel == i)
			digitalWrite(m_dipin, HIGH);
		else
			digitalWrite(m_dipin, LOW);

		delayMicroseconds(50);

		digitalWrite(m_clockpin, HIGH);
		delayMicroseconds(50);
		digitalWrite(m_clockpin, LOW);
		delayMicroseconds(50);
	}

	delayMicroseconds(400);

	uint32_t u32data = 0;
	for (int i = 0; i < 8; i++)
	{
		u32data <<= 1; // shift bit

		digitalWrite(m_clockpin, HIGH);
		delayMicroseconds(50);
		digitalWrite(m_clockpin, LOW);
		delayMicroseconds(50);

		if (digitalRead(m_dopin))
		{
			u32data |= 0x1; // set first bit
		}
	}

	digitalWrite(m_cspin, HIGH);

	m_sample_data.set_data(u32data, ERROR_SUCCESS);

	return m_sample_data.error_code();
}

const sensor_data& adc832::get_data()
{
	return m_sample_data;
}

std::error_code adc832::set_data(const sensor_data& data)
{
	return std::make_error_code(std::errc::not_supported);
}

