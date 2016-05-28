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

pEncoder *pEncoder::m_instance = nullptr;

pEncoder::pEncoder(const uint8_t id)
:
	AbEncoder({ 0 }),
	m_encoderCountToCm(pResource::configTable->kEncoderCountToCm),
	m_averageCountPerS(0),
	m_lastTime(0)
{
	System::Init();

	if (!m_instance)
		m_instance = this;
}

void pEncoder::updateEncoder(void)
{
	m_instance->m_averageCountPerS = m_instance->GetCount() * (System::Time() - m_instance->m_lastTime) / 1000;
}

uint32_t pEncoder::getSpeedMs(void) const
{
	// m_averageCountPerS * m_encoderCountToCm / 100 = SpeedMs
	return m_averageCountPerS * m_encoderCountToCm / 100;
}

uint32_t pEncoder::getSpeedCount(void) const
{
	return GetCount();
}
