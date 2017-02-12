
/****************************************************************************************************************/
/*                                                                                                              */ 
/*                                                                                                              */
/*   Bogdan Mihalcea                                                                                            */ 
/*   bogdanmihalcea@yahoo.com                                                                                   */
/*                                                                                                              */  
/****************************************************************************************************************/

#pragma once
#include "pch.h"
#include "sensor_transmission_base.h"

class one_wire_base :
	public sensor_transmission_base
{
protected:
	one_wire_base(std::string name, uint32_t dopin, uint32_t dipin);
	void begin_receive();
	void end_receive();
	bool check_crc(const uint8_t *databuf, size_t count, uint8_t crc);
};


