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

	struct Config
	{
		const uint8_t motorId;
		const uint8_t encoderId;
		bool isMotorInverse;
		bool isEncoderrInverse;
		MappingFunc mappingFunction;
		float &kP;
		float &kI;
		float &kD;

		Config(const uint8_t _motorId, const uint8_t _encoderId, bool _isMotorInverse, bool _isEncoderrInverse, MappingFunc _mappingFunction, float &_kP, float &_kI, float &_kD)
		:
			motorId(_motorId),
			encoderId(_encoderId),
			isMotorInverse(_isMotorInverse),
			isEncoderrInverse(_isEncoderrInverse),
			mappingFunction(_mappingFunction),
			kP(_kP),
			kI(_kI),
			kD(_kD)
		{}
	};

	pMotor(Config config);

	void update(const float angle);

	void setPower(const int16_t power);
	int16_t getPower(void);

	float getSpeedMs(void) const;
	float getSpeedCount(void) const;

	void setSetPoint(const float newSetPoint);

private:

	pEncoder			m_encoder;
	pPid				m_pid;

	bool				m_isInverse;
	float				m_setPoint;

};
