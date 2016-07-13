/*
 *	pPid.cpp
 *	
 *	Author: petel__
 *	Copyright (c) 2014-2016 HKUST SmartCar Team
 *	Refer to LICENSE for details
 *	
 */

#include "pPid.h"

using namespace libsc;

pPid::pPid(pPid::PidParam param)
:
	m_param(param),
	m_lastTime(0),
	m_lastSetPoint(0.0f),
	m_lastError(0.0f),
	m_lastSumError(0.0f),
	m_sum(0.0f)
{
	System::Init();
}

void pPid::reset(void)
{
	m_lastSetPoint = 0.0f;
	m_lastSumError = 0.0f;
	m_sum = 0;
	m_lastTime = 0;
}

float pPid::getOutput(const float val)
{
	float tempOutput = 0;

	if (m_lastTime && !isInRange(*m_param.setPoint - m_param.ignoreRange, val, *m_param.setPoint + m_param.ignoreRange))
	{
		float error = 0.0f;
		if (m_param.setPoint)
			error = *m_param.setPoint - val;
		else
			error = 0.0f;
		float dt = (System::Time() - m_lastTime) / 1000.0f;

		tempOutput +=  ((m_param.kPFunc)? m_param.kPFunc(error, error, *m_param.kP) : error * *m_param.kP);

		tempOutput +=  ((m_param.kDFunc)? m_param.kDFunc(error, (error - m_lastError) / dt, *m_param.kD) : (error - m_lastError) / dt * *m_param.kD);

		m_sum = inRange(m_param.sumMin, m_sum + (m_lastSetPoint - val - m_lastSumError) * dt * 0.5f, m_param.sumMax);

		tempOutput += (m_param.kIFunc)? m_param.kIFunc(error, m_sum, *m_param.kI) : m_sum * *m_param.kI;

		m_lastError = error;
	}

	m_lastSetPoint = *m_param.setPoint;
	m_lastSumError = m_lastSetPoint - val;
	m_lastTime = System::Time();

	return inRange(m_param.outputMin, tempOutput, m_param.outputMax);
}

float &pPid::getSum(void)
{
	return m_sum;
}
