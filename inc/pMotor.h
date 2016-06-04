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
	void update(void);

	void setPower(const int16_t power);
	void setSpeed(const int16_t power);
	int16_t &getPower(void);

	int32_t &getSpeedCount(void);

	void setSetPoint(const float newSetPoint);

	void setKp(const float kP);
	float getKp(void);
	void setKi(const float kI);
	float getKi(void);
	void setKd(const float kD);
	float getKd(void);

private:

	Config				m_config;
	pPid::ConstFunc	m_kPFunc;

	pEncoder			m_encoder;
	pPid				m_pid;

	bool				m_isInverse;
	float				m_setPoint;
	int16_t				m_lastPower;

};
