/*
 * pLowPassFilter.cpp
 *
 * Author: Petel__
 * Copyright (c) 2014-2016 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include <pLowPassFilter.h>

using namespace std;
using namespace libsc;

pLowPassFilter::pLowPassFilter(float Beta)
:
	m_lastOutput(0.0f),
	m_beta(Beta)
{}

pLowPassFilter::pLowPassFilter(Timer::TimerInt dt, float timeConst)
:
	m_lastOutput(0.0f),
	m_beta(dt / (timeConst + dt))
{}

float pLowPassFilter::filter(const float val)
{
	return (m_lastOutput = m_lastOutput + m_beta * (val - m_lastOutput));
}
