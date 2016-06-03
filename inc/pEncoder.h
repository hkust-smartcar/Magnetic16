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

	pEncoder(const uint8_t id, bool isClockwise);

	void update(void);

	float &getSpeedMs(void);
	int32_t &getSpeedCount(void);

private:

	int32_t						m_lastCount;
	float						m_lastMs;
	const int32_t				m_encoderCountToCm;
	float 						m_averageCountPerS;
	Timer::TimerInt				m_lastTime;

	bool						m_isInverse;

};
