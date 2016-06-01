/*
 *	pPid.h
 *	
 *	Author: petel__
 *	Copyright (c) 2014-2016 HKUST SmartCar Team
 *	Refer to LICENSE for details
 *	
 */

#pragma once

#include <stdint.h>
#include <libsc/system.h>
#include <libsc/timer.h>

#define MAX(a, b) ((a > b)? a : b)
#define inRange(n, v, x) ((v < n)? n : ((v > x)? x : v))
#define ABS(v) ((v > 0)? v : -v)

using namespace libsc;

class pPid
{

public:

	struct PidParam
	{
		float &kP;
		float &kI;
		float &kD;
		float &setPoint;

		float max;
		float min;

		bool useStrict = true;

		PidParam(float &_kP, float &_kI, float &_kD, float &_setPoint, float _max, float _min)
		:
			kP(_kP), kI(_kI), kD(_kD), setPoint(_setPoint), max(_max), min(_min)
		{}

		PidParam(const PidParam &other)
		:
			kP(other.kP), kI(other.kI), kD(other.kD), setPoint(other.setPoint), max(other.max), min(other.min)
		{}
	};

	pPid(PidParam param);

	void reset(void);

	float getOutput(const float val);

private:

	PidParam			m_param;

	Timer::TimerInt		m_lastTime;
	int32_t				m_lastError;
	int32_t				m_sum;

	const float		m_epsilon;

};
