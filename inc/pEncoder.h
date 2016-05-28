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

	// 1000000 => 1 (m/s)
	uint32_t getSpeedMs(void) const;
	uint32_t getSpeedCount(void) const;

private:

	// 1000000 => 1 (cm / count)
	const uint32_t				m_encoderCountToCm;
	uint32_t 					m_averageCountPerS;
	Timer::TimerInt				m_lastTime;

	static pEncoder	*m_instance;

};
