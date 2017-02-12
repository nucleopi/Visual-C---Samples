/****************************************************************************************************************/
/*                                                                                                              */ 
/*                                                                                                              */
/*   Bogdan Mihalcea                                                                                            */ 
/*   bogdanmihalcea@yahoo.com                                                                                   */
/*                                                                                                              */  
/****************************************************************************************************************/

#include "sensor_base.h"

sensor_base::sensor_base()
{
	init();
}

sensor_base::~sensor_base()
{
}

std::error_code sensor_base::init()
{
	if (!is_inited && -1 == wiringPiSetup())
	{
		return std::make_error_code(std::errc::timed_out);
	}

	is_inited = true;

	return ERROR_SUCCESS;
}

void sensor_base::set_debug_mode(bool mode)
{
	debug_mode = mode;
}

std::shared_ptr<std::string> sensor_base::name() const
{
	return m_name;
}
