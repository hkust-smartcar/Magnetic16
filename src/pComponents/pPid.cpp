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

bool pPid::isReady(void)
{
	int32_t dt = System::Time() - m_lastTime - m_param.updatePeriod;

	assert(dt <= 0);

	return (dt == 0); // TODO: check if this works
}

int32_t pPid::getOutput(const int32_t val)
{
	int32_t tempOutput = 0;

	if (m_lastTime)
	{

		int32_t error = m_param.setPoint - val;
		uint32_t dt = System::Time() - m_lastTime;

		tempOutput += error * m_param.kP;

		tempOutput += (error - m_lastError) * 1000 / dt * m_param.kD;

		if (error > m_epsilon)
		{
			m_sum += ((error + m_lastError) * dt) >> 1;
			tempOutput += m_sum * m_param.kI;
		}

		m_lastError = error;
	}

	m_lastTime = System::Time();

	return tempOutput;
}
