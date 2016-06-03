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
	m_lastError(0),
	m_sum(0),
	m_epsilon(m_param.max * 15 / 100)
{
	System::Init();
}

void pPid::reset(void)
{
	m_sum = 0;
	m_lastTime = 0;
}

float pPid::getOutput(const float val)
{
	float tempOutput = 0;

	if (m_lastTime)
	{

		float error = *m_param.setPoint - val;
		uint32_t dt = System::Time() - m_lastTime;

		tempOutput +=  ((m_param.kPFunc)? m_param.kPFunc(error) : *m_param.kP);

		tempOutput += (error - m_lastError) * 1000 / dt * ((m_param.kDFunc)? m_param.kDFunc(error) : *m_param.kD);

		if (error > m_epsilon)
		{
			m_sum += ((error + m_lastError) * dt) * 0.5f;
			tempOutput += m_sum * ((m_param.kIFunc)? m_param.kIFunc(error) : *m_param.kI);
		}

		m_lastError = error;
	}

	m_lastTime = System::Time();

	return inRange(m_param.min, tempOutput, m_param.max);
}
