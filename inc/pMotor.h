/*
 *	pMotor.h
 *	
 *	Author: petel__
 *	Copyright (c) 2014-2016 HKUST SmartCar Team
 *	Refer to LICENSE for details
 *	
 */

#pragma once

#include <libsc/dir_motor.h>
#include "pEncoder.h"

using namespace libsc;

class pMotor : private DirMotor
{

public:

	pMotor(const uint8_t id);

	int32_t getSpeedMs(void) const;
	void setSpeedMs(const int32_t speed);

	int32_t getSpeedCount(void) const;
	void setSpeedCount(const int32_t speed);

private:

	pEncoder			m_encoder;

};
