/*
 * MyEncoder.h
 *
 * Author: Peter
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <libsc/ab_encoder.h>
#include "MyResource.h"

using namespace libsc;

class MyEncoder : private AbEncoder
{

public:

	MyEncoder(void);

	int32_t getEncoderReading(void);
	int32_t *getEncoderCountPointer(void);

	void reset(void);

private:

	int32_t			m_lastCount;
	Timer::TimerInt	m_lastTime;

};
