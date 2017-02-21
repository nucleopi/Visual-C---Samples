#include "adc832.h"



adc832::adc832()
{
}

adc832::adc832(std::string name, uint32_t dopin, uint32_t dipin, uint32_t cspin, uint32_t clockpin, uint32_t channel)
{
	m_name = std::make_shared<std::string>(name);
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

uint32_t adc832::read_adc()
{
	pinMode(m_dipin, OUTPUT);
	pinMode(m_dopin, INPUT);
	pinMode(m_cspin, OUTPUT);
	pinMode(m_clockpin, OUTPUT);


	digitalWrite(m_cspin, HIGH); //clear last transmission
	digitalWrite(m_cspin, LOW);



	digitalWrite(m_clockpin, LOW); //start clock low

	// using ADC 832 which has 2 channels
	for (int i = 0; i < 2; i++)
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

	uint32_t val = 0;
	for (int i = 0; i < 8; i++)
	{
		val <<= 1; // shift bit

		digitalWrite(m_clockpin, HIGH);
		delayMicroseconds(50);
		digitalWrite(m_clockpin, LOW);
		delayMicroseconds(50);

		if (digitalRead(m_dopin))
		{		
			val |= 0x1; // set first bit
		}
	}

	digitalWrite(m_cspin, HIGH);

	return val;

}

std::shared_ptr<std::string> adc832::name() const
{
	return std::shared_ptr<std::string>(new std::string("adc832"));
}
