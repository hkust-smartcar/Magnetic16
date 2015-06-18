/*
 * MyMagSen.cpp
 *
 * Author: Peter
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include <array>
#include <libbase/k60/pin.h>
#include <libbase/k60/adc.h>
#include "MyMagSen.h"
#include "MyResource.h"
#include "MyKalmanFilter.h"

using namespace std;
using namespace libbase::k60;

inline Pin::Name getPin(const uint8_t id)
{
	switch (id)
	{
	default:
		assert(false);
		return Pin::Name::kDisable;

#ifdef LIBSC_USE_MAGSEN
	case 0:
		return LIBSC_MAGSEN5;
#endif

#if LIBSC_USE_MAGSEN > 1
	case 1:
		return LIBSC_MAGSEN4;
#endif

#if LIBSC_USE_MAGSEN > 2
	case 2:
		return LIBSC_MAGSEN3;
#endif

#if LIBSC_USE_MAGSEN > 3
	case 3:
		return LIBSC_MAGSEN2;
#endif

#if LIBSC_USE_MAGSEN > 4
	case 4:
		return LIBSC_MAGSEN1;
#endif

#if LIBSC_USE_MAGSEN > 5
	case 5:
		return LIBSC_MAGSEN0;
#endif
	}
}

Adc::Config getAdcConfig(MyMagSen::MagSen type, MyMagSen::Side which)
{
	Adc::Config config;
	config.avg_pass = Adc::Config::AveragePass::k16;
	config.speed = Adc::Config::SpeedMode::kSlow;
	config.resolution = Adc::Config::Resolution::k16Bit;
	config.pin = getPin((uint8_t)(type << 1) + (uint8_t)which);
	return config;
}

MyMagSen::MyMagSen(MagSen type)
:
	m_magSen({ Adc(getAdcConfig(type, Side::LEFT)), Adc(getAdcConfig(type, Side::RIGHT)) }),
	m_filter({ MyKalmanFilter(MyResource::ConfigTable::MagSenConfig::Kq,
							  MyResource::ConfigTable::MagSenConfig::Kr,
							  0.5f, 0.5f),
			   MyKalmanFilter(MyResource::ConfigTable::MagSenConfig::Kq,
					   	      MyResource::ConfigTable::MagSenConfig::Kr,
							  0.5f, 0.5f)
			 }),
			 m_output(0.0f)
{}

void MyMagSen::reset(void)
{
	m_output = 0.0f;
}

float MyMagSen::getValue(void)
{
	m_rawReading[Side::LEFT] = m_magSen[Side::LEFT].GetResultF();
	m_rawReading[Side::RIGHT] = m_magSen[Side::RIGHT].GetResultF();
	m_filteredReading[Side::LEFT] = m_filter[Side::LEFT].Filter(m_rawReading[Side::LEFT]);
	m_filteredReading[Side::RIGHT] = m_filter[Side::RIGHT].Filter(m_rawReading[Side::RIGHT]);
	m_output = (m_filteredReading[Side::LEFT] - m_filteredReading[Side::RIGHT]) / (m_filteredReading[Side::LEFT] + m_filteredReading[Side::RIGHT]);
//	m_output = (m_rawReading[Side::LEFT] - m_rawReading[Side::RIGHT]) / (m_rawReading[Side::LEFT] + m_rawReading[Side::RIGHT]);
	m_avg = (m_filteredReading[Side::LEFT] + m_filteredReading[Side::RIGHT]) / 2.0f;
	return m_output;
}

float &MyMagSen::getOutputValue(void)
{
	return m_output;
}

array<float, 2> &MyMagSen::getFilteredValue(void)
{
	return m_filteredReading;
}

float &MyMagSen::getFilteredValueAvg(void)
{
	return m_avg;
}

array<float, 2> &MyMagSen::getRawValue(void)
{
	return m_rawReading;
}
