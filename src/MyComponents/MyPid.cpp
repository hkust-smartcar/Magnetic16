/*
 * MyPid.cpp
 *
 * Author: Peter
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include <cstring>

#include <libsc/system.h>
#include <libsc/timer.h>
#include <libutil/math.h>
#include <MyPid.h>

#include "MySmartCar.h"
#include "MyResource.h"

using namespace libsc;

#define abs(v) ((v > 0)? v : -v)

MyPid::MyPid(float &ref, float &kp, float &ki, float &kd, const Type type, const float outputMin, const float outputMax, const float errorMin, const float errorMax, float &nonLinearKp_squ_a, float &nonLinearKp_b)
:
	m_outputMin(outputMin),
	m_outputMax(outputMax),
	m_errorMin(errorMin),
	m_errorMax(errorMax),
	m_errorRange((errorMax - errorMin) / 2),

	m_reference(ref),
	m_Kp(kp),
	m_Ki(ki),
	m_Kd(kd),

	m_Kp_squ_a(nonLinearKp_squ_a),
	m_Kp_b(nonLinearKp_b),

	m_type(type),

	m_eSum(0),
	m_lastError(0),
	m_epsilon(m_reference * EPSILON_RATIO),
	m_output(0)
{
	System::Init();
	reset();
}

float &MyPid::getKp(void)
{
	return m_Kp;
}

float &MyPid::getKi(void)
{
	return m_Ki;
}

float &MyPid::getKd(void)
{
	return m_Kd;
}

float *MyPid::getDerivative(void)
{
	return &m_eDer;
}

float *MyPid::getIntegral(void)
{
	return &m_eSum;
}

float *MyPid::getLastError(void)
{
	return &m_lastError;
}

void MyPid::reset(void)
{
	m_lastTimeUpdate = System::Time() - MyResource::ConfigTable::MotorConfig::UpdateFreq;
	m_output = 0;
	m_lastError = 0;
	m_eSum = 0;
}

float MyPid::update(float val)
{
	float error = m_reference - val;
	uint32_t dt = System::Time() - m_lastTimeUpdate;
	m_eDer = (error - m_lastError) / (((float)dt)/1000.0f);
	m_lastError = error;

	m_lastTimeUpdate = System::Time();

	switch (m_type)
	{
	case Type::Motor:

		if (abs(m_lastError) >= m_epsilon)
			m_eSum += error*dt;

		return updateMotorPID();

	case Type::ServoLinear:

		return updateServoPID();

	case Type::ServoNonLinear:

		return updateNonLinearPID();

	default:
		assert(false);
		return 0.0f;

	}
}

float MyPid::updateMotorPID(void)
{
	m_output += m_Kp * m_lastError + m_Ki * m_eSum + m_Kd * m_eDer;

	return inRange(m_outputMin, m_output, m_outputMax);
}

float MyPid::updateServoPID(void)
{
	m_output = m_Kp * m_lastError + m_Ki * m_eSum + m_Kd * m_eDer;

	return inRange(m_outputMin, m_output, m_outputMax);
}

float MyPid::updateNonLinearPID(void)
{
	float p = m_lastError * ABS(m_lastError);
	m_output = (m_Kp_squ_a * p) + m_Ki * m_eSum + m_Kd * m_eDer;

	return inRange(m_outputMin, m_output, m_outputMax);
}
