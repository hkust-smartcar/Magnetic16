/*
 * pMagSen.cpp
 *
 * Author: Petel__
 * Copyright (c) 2014-2016 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include <pMagSen.h>
#include <libsc/config.h>

using namespace std;
using namespace libbase::k60;

Adc::Config getAdcConfig(libbase::k60::Pin::Name pin)
{
	Adc::Config config;
	config.pin = pin;
	config.avg_pass = Adc::Config::AveragePass::k8;
	config.speed = Adc::Config::SpeedMode::kTypical;
	config.resolution = Adc::Config::Resolution::k16Bit;
	return config;
}

libbase::k60::Pin::Name getAdcPinNameByIndex(const uint8_t index)
{
	assert(index < LIBSC_USE_MAGSEN);

	switch (index)
	{
#if LIBSC_USE_MAGSEN > 0
	case 0:
		return LIBSC_MAGSEN0;
#endif

#if LIBSC_USE_MAGSEN > 1
	case 1:
		return LIBSC_MAGSEN1;
#endif

#if LIBSC_USE_MAGSEN > 2
	case 2:
		return LIBSC_MAGSEN2;
#endif

#if LIBSC_USE_MAGSEN > 3
	case 3:
		return LIBSC_MAGSEN3;
#endif

#if LIBSC_USE_MAGSEN > 4
	case 4:
		return LIBSC_MAGSEN4;
#endif

#if LIBSC_USE_MAGSEN > 5
	case 5:
		return LIBSC_MAGSEN5;
#endif
	}
}

pMagSen::pMagSen(const uint8_t startIndex, const bool isPair)
:
	m_magSenPair{ Adc(nullptr), Adc(nullptr) },
	m_lastResultPair{ 0.0f, 0.0f },
	m_magSen(nullptr),
	m_lastResult(0.0f),
	m_isPair(isPair)
{
	if (m_isPair)
		m_magSenPair = array<Adc, 2>{ Adc(getAdcConfig(getAdcPinNameByIndex(startIndex))), Adc(getAdcConfig(getAdcPinNameByIndex(startIndex + 1))) };
	else
		m_magSen = Adc(getAdcConfig(getAdcPinNameByIndex(startIndex)));
}

float pMagSen::updatePair(void)
{
	if (m_isPair)
	{
		m_lastResultPair[0] = m_magSenPair[0].GetResultF();
		m_lastResultPair[1] =  m_magSenPair[1].GetResultF();
		return ((m_lastResultPair[0] - m_lastResultPair[1]) / (m_lastResultPair[0] + m_lastResultPair[1]));
	}
	else
		return 0.0f;
}

float pMagSen::update(void)
{
	if (!m_isPair)
		return (m_lastResult = m_magSen.GetResultF());
	else
		return 0.0f;
}

float pMagSen::getResult(void) const
{
	return m_lastResult;
}

float &pMagSen::getVoltage(const uint8_t index)
{
	if (m_isPair)
		return m_lastResultPair[index];
	else
		return m_lastResult;
}

