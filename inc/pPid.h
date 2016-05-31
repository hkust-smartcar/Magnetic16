/*
 *	pPid.h
 *	
 *	Author: petel__
 *	Copyright (c) 2014-2016 HKUST SmartCar Team
 *	Refer to LICENSE for details
 *	
 */

#pragma once

#include <cassert>
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

	// 10000 => 1.0
	struct PidParam
	{
		int32_t &kP;
		int32_t &kI;
		int32_t &kD;
		int32_t &setPoint;

		int32_t max;
		int32_t min;

		uint32_t updatePeriod;
		bool useStrict = true;

		PidParam(int32_t &_kP, int32_t &_kI, int32_t &_kD, int32_t &_setPoint, int32_t _max, int32_t _min, uint32_t _updatePeriod)
		:
			kP(_kP), kI(_kI), kD(_kD), setPoint(_setPoint), max(_max), min(_min), updatePeriod(_updatePeriod)
		{}

		PidParam(const PidParam &other)
		:
			kP(other.kP), kI(other.kI), kD(other.kD), setPoint(other.setPoint), max(other.max), min(other.min), updatePeriod(other.updatePeriod)
		{}
	};

	pPid(void) = delete;
	pPid(PidParam param);

	void reset(void);
	bool isReady(void);

	// val: 10000 => 1.0
	int32_t getOutput(const int32_t val);

private:

	PidParam			m_param;

	Timer::TimerInt		m_lastTime;
	int32_t				m_lastError;
	int32_t				m_sum;

	const int32_t		m_epsilon;

};
