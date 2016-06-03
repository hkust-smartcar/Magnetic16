/*
 *	pPid.h
 *	
 *	Author: petel__
 *	Copyright (c) 2014-2016 HKUST SmartCar Team
 *	Refer to LICENSE for details
 *	
 */

#pragma once

#include <functional>
#include <stdint.h>
#include <libsc/system.h>
#include <libsc/timer.h>

#define MAX(a, b) ((a > b)? a : b)
#define inRange(n, v, x) ((v < n)? n : ((v > x)? x : v))
#define ABS(v) ((v > 0)? v : -v)

using namespace libsc;
using namespace std;

class pPid
{

public:

	typedef function<float (float)> ConstFunc;

	struct PidParam
	{
		float *kP = nullptr;
		float *kI = nullptr;
		float *kD = nullptr;
		float *setPoint = nullptr;

		ConstFunc kPFunc;
		ConstFunc kIFunc;
		ConstFunc kDFunc;

		float max;
		float min;

		bool useStrict = true;
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
