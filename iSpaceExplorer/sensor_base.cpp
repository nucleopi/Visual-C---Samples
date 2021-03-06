/****************************************************************************************************************/
/*                                                                                                              */
/*   Copyright (c) Bogdan Mihalcea 2017                                                                         */
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

void sensor_base::set_verbosity(bool mode)
{
	verbosity_mode = mode;
}

const std::string sensor_base::name()
{
	return m_name;
}
