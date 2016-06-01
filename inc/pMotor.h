/*
 *	pMotor.h
 *	
 *	Author: petel__
 *	Copyright (c) 2014-2016 HKUST SmartCar Team
 *	Refer to LICENSE for details
 *	
 */

#pragma once

#include <functional>
#include <libsc/dir_motor.h>
#include "pEncoder.h"
#include <pPid.h>

#define ABS(v) ((v < 0)? -v : v)

using namespace std;
using namespace libsc;

class pMotor : private DirMotor
{

public:

	typedef function<float (const float)> MappingFunc;

	pMotor(const uint8_t id, MappingFunc mapingFunction, float &kP, float &kI, float &kD);

	void update(void);

	float getSpeedMs(void) const;
	void setSpeedMs(const int32_t speed);

	float getSpeedCount(void) const;
	void setSpeedCount(const int32_t speed);

private:

	pEncoder			m_encoder;
	pPid				m_pid;

	float				m_setPoint;

};
