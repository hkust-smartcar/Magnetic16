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

	void update(void);

	float getSpeedMs(void) const;
	float getSpeedCount(void) const;

private:

	const float					m_encoderCountToCm;
	float 						m_averageCountPerS;
	Timer::TimerInt				m_lastTime;

};
