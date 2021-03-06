/*
 * pMagSen.h
 *
 * Author: Petel__
 * Copyright (c) 2014-2016 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <array>
#include <cassert>
#include <libbase/k60/adc.h>
#include <pLowPassFilter.h>

using namespace std;
using namespace libbase::k60;

class pMagSen
{

public:

	pMagSen(const uint8_t startIndex, const bool isPair = true, const float factor = 40.0f, const Timer::TimerInt dt = 10);

	float updatePair(void);
	float update(void);

	float &getVoltage(const uint8_t index = 0);
	float &getResult(void);

private:

	array<Adc, 2>				m_magSenPair;
	array<float, 2>				m_lastResultPair;

	pLowPassFilter				m_lpf;

	Adc							m_magSen;
	float						m_lastResult;

	bool						m_isPair;

};
