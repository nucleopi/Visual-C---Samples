#pragma once
#include "ir_base.h"
class ir_generic :
	public ir_base
{
public:
	ir_generic(std::string name, uint32_t dopin, uint32_t dipin);
	~ir_generic();
	virtual sensor_data read();
	virtual std::error_code write(sensor_data& data);
	virtual std::shared_ptr<std::string> name() const;
};

