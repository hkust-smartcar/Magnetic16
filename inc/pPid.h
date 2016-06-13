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
#define isInRange(n, v, x) (v >= n && v <= x)
#define ABS(v) ((v > 0)? v : -v)

using namespace libsc;
using namespace std;

class pPid
{

public:

	typedef function<float (float error, float additionalInfo, float constant)> ConstFunc;

	struct PidParam
	{
		float *kP = nullptr;
		float *kI = nullptr;
		float *kD = nullptr;
		float *setPoint = nullptr;

		ConstFunc kPFunc;
		ConstFunc kIFunc;
		ConstFunc kDFunc;

		float ignoreRange = 0.0f;

		float outputMax;
		float outputMin;

		float sumMax;
		float sumMin;

		bool useStrict = true;
	};

	pPid(PidParam param);

	void reset(void);

	float getOutput(const float val);

	float &getSum(void);

private:

	PidParam			m_param;

	Timer::TimerInt		m_lastTime;
	float				m_lastError;
	float				m_sum;

	const float		m_epsilon;

};
