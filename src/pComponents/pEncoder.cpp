/*
 *	pEncoder.cpp
 *	
 *	Author: petel__
 *	Copyright (c) 2014-2016 HKUST SmartCar Team
 *	Refer to LICENSE for details
 *	
 */

#include "pEncoder.h"
#include "pResource.h"

using namespace libsc;

pEncoder::pEncoder(const uint8_t id, bool isInverse)
:
	AbEncoder({ id }),
	m_averageCountPerS(0),
	m_lastTime(0),
	m_isInverse(isInverse)
{
	System::Init();
}

void pEncoder::update(void)
{
	Update();
	m_lastCount = (m_isInverse)? -GetCount() : GetCount();
	//m_averageCountPerS = m_lastCount * (System::Time() - m_lastTime) / 1000;
}

void pEncoder::reset(void)
{
	update();
	update();
	m_averageCountPerS = 0;
	m_lastTime = 0;
	m_lastCount = 0;
}

int16_t &pEncoder::getEncoderCount(void)
{
	return m_lastCount;
}
