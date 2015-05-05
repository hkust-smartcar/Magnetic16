/*
 * MyPID.cpp
 *
 * Author: Peter
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include <cstring>

#include <libsc/system.h>
#include <libsc/timer.h>

#include "MyPID.h"
#include "MySmartCar.h"
#include "MyResource.h"

using namespace libsc;

#define abs(v) ((v > 0)? v : -v)

MyPID::MyPID(float &ref, float &kp, float &ki, float &kd, const float outputMin, const float outputMax, const float errorMin, const float errorMax)
:
	m_outputMin(outputMin),
	m_outputMax(outputMax),
	m_errorRange(outputMax - outputMin),
	m_errorMin(errorMin),
	m_errorMax(errorMax),

	m_reference(ref),
	m_Kp(kp),
	m_Ki(ki),
	m_Kd(kd),

	m_eSum(0),
	m_lastError(0),
	m_epsilon(m_reference * EPSILON_RATIO),
	m_output(0)
{
	System::Init();
	reset();
}

float &MyPID::getKp(void)
{
	return m_Kp;
}

float &MyPID::getKi(void)
{
	return m_Ki;
}

float &MyPID::getKd(void)
{
	return m_Kd;
}

float *MyPID::getDerivative(void)
{
	return &m_eDer;
}

float *MyPID::getIntegral(void)
{
	return &m_eSum;
}

float *MyPID::getLastError(void)
{
	return &m_lastError;
}

void MyPID::reset(void)
{
	m_lastTimeUpdate = System::Time();
	m_output = 0;
	m_lastError = 0;
	m_eSum = 0;
}

float MyPID::updateMotorPID(float val)
{
	float error = m_reference - val;
	uint32_t dt = System::Time() - m_lastTimeUpdate;
	m_eDer = (error - m_lastError) / dt;
	m_lastError = error;

	if (abs(m_lastError) >= m_epsilon)
		m_eSum += error*dt;

	m_output += m_Kp * m_lastError + m_Ki * m_eSum + m_Kd * m_eDer;

	m_lastTimeUpdate = System::Time();

	return inRange(m_outputMin, m_output, m_outputMax);
}

float MyPID::updateServoPID(float val)
{
	float error = m_reference - val;
	uint32_t dt = System::Time() - m_lastTimeUpdate;
	m_eDer = (error - m_lastError) / dt;
	m_lastError = error;

	m_output = m_Kp * m_lastError + m_Ki * m_eSum + m_Kd * m_eDer;

	m_lastTimeUpdate = System::Time();

	return inRange(m_outputMin, m_output, m_outputMax);
}

float MyPID::updateNonLinearPID(float val)
{
	float error = m_reference - val;
	uint32_t dt = System::Time() - m_lastTimeUpdate;
	m_eDer = (error - m_lastError) / dt;
	m_lastError = error;

	m_output = m_Kp * (ABS(error) / m_errorRange) * m_lastError + m_Ki * m_eSum + m_Kd * m_eDer;

	m_lastTimeUpdate = System::Time();

	return inRange(m_outputMin, m_output, m_outputMax);
}
