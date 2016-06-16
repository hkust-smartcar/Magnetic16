/*
 * pLowPassFilter.h
 *
 * Author: Petel__
 * Copyright (c) 2014-2016 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <libsc/timer.h>

using namespace std;
using namespace libsc;

class pLowPassFilter
{

public:

	pLowPassFilter(float Beta);
	pLowPassFilter(Timer::TimerInt dt, float timeConst);

	float filter(const float val);

private:

	float			m_lastOutput;

	float			m_beta;

};

