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

pEncoder::pEncoder(const uint8_t id)
:
	AbEncoder({ 0 }),
	m_encoderCountToCm(pResource::configTable->kEncoderCountToCm),
	m_averageCountPerS(0),
	m_lastTime(0)
{
	System::Init();
}

void pEncoder::update(void)
{
	m_averageCountPerS = GetCount() * (System::Time() - m_lastTime) / 1000;
}

float pEncoder::getSpeedMs(void) const
{
	// m_averageCountPerS * m_encoderCountToCm / 100 = SpeedMs
	return m_averageCountPerS * m_encoderCountToCm / 100;
}

float pEncoder::getSpeedCount(void) const
{
	return GetCount();
}
