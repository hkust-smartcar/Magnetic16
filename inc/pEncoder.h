/*
 *	pEncoder.h
 *	
 *	Author: petel__
 *	Copyright (c) 2014-2016 HKUST SmartCar Team
 *	Refer to LICENSE for details
 *	
 */

#pragma once

#include <libsc/system.h>
#include <libsc/timer.h>
#include <libsc/ab_encoder.h>

using namespace libsc;

class pEncoder : private AbEncoder
{

public:

	pEncoder(const uint8_t id);

	static void updateEncoder(void);

	// Ms: 1024 => 1.0 (m/s)
	uint32_t getSpeedMs(void) const;
	uint32_t getSpeedCount(void) const;

private:

	// Ms: 1024 => 1.0 (m/s) / 100 count
	const uint32_t		m_encoderCountToMs;
	// CountPerS: 1024 -> 100 count/s
	uint32_t 					m_averageCountPerS;
	Timer::TimerInt				m_lastTime;

};
