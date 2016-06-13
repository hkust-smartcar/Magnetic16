/*
 *	pMotor.h
 *	
 *	Author: petel__
 *	Copyright (c) 2014-2016 HKUST SmartCar Team
 *	Refer to LICENSE for details
 *	
 */

#pragma once

#include <array>
#include <functional>
#include <cmath>
#include <libsc/dir_motor.h>
#include "pEncoder.h"
#include <pPid.h>

#define ABS(v) ((v < 0)? -v : v)
#define DegToRad 0.017453293f

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

		Config(const uint8_t _motorId, const uint8_t _encoderId, bool _isMotorInverse, bool _isEncoderrInverse, MappingFunc _mappingFunction)
		:
			motorId(_motorId),
			encoderId(_encoderId),
			isMotorInverse(_isMotorInverse),
			isEncoderrInverse(_isEncoderrInverse),
			mappingFunction(_mappingFunction)
		{}
	};

	pMotor(Config config);

	void update(void);
	void reset(void);

	void setEnabled(const bool enabled);
	bool isEnabled(void);

	void setSpeed(const float rpt);
	void setPower(const float power);
	void setMappedPower(const float power);
	void setAccel(const float accel);

	int16_t &getPower(void);
	int16_t &getEncoderCount(void);

private:

	Config				m_config;

	pEncoder			m_encoder;

	bool				m_enabled;
	bool				m_isInverse;
	int16_t				m_lastPower;

};
