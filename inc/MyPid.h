/*
 * MyPid.h
 *
 * Author: Peter
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include <libsc/system.h>
#include "MyResource.h"

#pragma once

#define MAX(a, b) ((a > b)? a : b)
#define inRange(n, v, x) ((v < n)? n : ((v > x)? x : v))
#define ABS(v) ((v > 0)? v : -v)

#define EPSILON_RATIO			0.05

using namespace libsc;

class MyPid
{
public:

	enum Type
	{
		Motor = 0,
		ServoLinear,
		ServoNonLinear
	};

	explicit MyPid(float &ref, float &kp, float &ki, float &kd, const Type type, const float outputMin, const float outputMax, const float errorMin, const float errorMax, float nonLinearKp_a = 0.0f, float nonLinearKp_b = 0.0f);

	float update(float val);
	float updateMotorPID(void);
	float updateServoPID(void);
	float updateNonLinearPID(void);

	float &getKp(void);
	float &getKi(void);
	float &getKd(void);

	float *getDerivative(void);
	float *getIntegral(void);
	float *getLastError(void);

	void reset(void);

private:

	float m_outputMin;
	float m_outputMax;
	float m_errorMin;
	float m_errorMax;
	float m_errorRange;

	float &m_reference;
	float &m_Kp;
	float &m_Ki;
	float &m_Kd;

	float m_Kp_a;
	float m_Kp_b;

	Type m_type;

	float m_eDer;
	float m_eSum;

	float m_lastError;
	float m_epsilon;
	Timer::TimerInt m_lastTimeUpdate;

	float m_output;

};
