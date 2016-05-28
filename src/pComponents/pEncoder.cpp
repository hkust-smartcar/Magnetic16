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
	m_encoderCountToMs(pResource::configTable->kEncoderCountToMs),
	m_averageCountPerS(0),
	m_lastTime(0)
{
	System::Init();

	if (!m_instance)
		m_instance = this;
}

void pEncoder::updateEncoder(void)
{
	// m_averageCountPerS = Count * ms * 1024 / 1000 = Count * ms * 125 / 128
	m_instance->m_averageCountPerS = (m_instance->GetCount() * (System::Time() - m_instance->m_lastTime) * 125) >> 7;
}

uint32_t pEncoder::getSpeedMs(void) const
{
	// ret = averageCountPerS / 1024 * encoderCountToMs / 1024 * 1024 = averageCountPerS * encoderCountToMs / 1024
	return (m_averageCountPerS * m_encoderCountToMs) >> 10;
}
