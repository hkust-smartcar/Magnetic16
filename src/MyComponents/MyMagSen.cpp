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
#include "MySmartCar.h"
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

Adc::Config MyMagSen::getAdcConfig(MyMagSen::MagSenId type, MyMagSen::Side which)
{
	Adc::Config config;
	config.avg_pass = Adc::Config::AveragePass::k16;
	config.speed = Adc::Config::SpeedMode::kSlow;
	config.resolution = Adc::Config::Resolution::k16Bit;
	config.pin = getPin((uint8_t)(type << 1) + (uint8_t)which);
	return config;
}

MyMagSen::MyMagSen(MagSenId type)
:
	m_ratio({ 0.0f, 0.0f }),
	m_magSenId(type),
	m_magSen({ Adc(getAdcConfig(type, Side::LEFT)), Adc(getAdcConfig(type, Side::RIGHT)) }),
	m_filter({ MyKalmanFilter(MyResource::ConfigTable::MagSenConfig::Kq,
							  MyResource::ConfigTable::MagSenConfig::Kr,
							  0.5f, 0.5f),
			   MyKalmanFilter(MyResource::ConfigTable::MagSenConfig::Kq,
					   	   	  MyResource::ConfigTable::MagSenConfig::Kr,
							  0.5f, 0.5f)
			 }),
	 m_rawReading({ 0.0f, 0.0f }),
	 m_standardizedReading({ 0.0f, 0.0f }),
	 m_filteredReading({ 0.0f, 0.0f }),
	 m_avg(0.0f),
	 m_output(0.0f),
	 m_isInit(false)
{}

void MyMagSen::reset(void)
{
	m_output = 0.0f;
}

void MyMagSen::init(const float *ratio)
{
	for (uint8_t i = 0; i < 2; i++)
		m_ratio[i] = ratio[i];

	m_isInit = true;
}

float MyMagSen::update(void)
{
	m_rawReading[Side::LEFT] = m_magSen[Side::LEFT].GetResultF();
	m_rawReading[Side::RIGHT] = m_magSen[Side::RIGHT].GetResultF();

	if (m_isInit && m_magSenId != MagSenId::FD)
	{
		m_standardizedReading[Side::LEFT] = m_rawReading[Side::LEFT] * (m_ratio[Side::LEFT]);
		m_standardizedReading[Side::RIGHT] = m_rawReading[Side::RIGHT] * (m_ratio[Side::RIGHT]);
	}
	else
	{
		m_standardizedReading[Side::LEFT] = m_rawReading[Side::LEFT];
		m_standardizedReading[Side::RIGHT] = m_rawReading[Side::RIGHT];
	}

	m_filteredReading[Side::LEFT] = m_filter[Side::LEFT].Filter(m_standardizedReading[Side::LEFT]);
	m_filteredReading[Side::RIGHT] = m_filter[Side::RIGHT].Filter(m_standardizedReading[Side::RIGHT]);

	m_avg = (m_standardizedReading[Side::LEFT] + m_standardizedReading[Side::RIGHT]) / 2.0f;

	switch (m_magSenId)
	{
	case MagSenId::SD:
		if (m_avg < MyResource::ConfigTable::ServoConfig::SdNoSignalThreshold)
			return m_output;

	case MagSenId::HD:
	case MagSenId::FD:
		m_output = (m_standardizedReading[Side::LEFT] - m_standardizedReading[Side::RIGHT]) / (m_standardizedReading[Side::LEFT] + m_standardizedReading[Side::RIGHT]);
		return m_output;

	}

}

float &MyMagSen::getOutputValue(void)
{
	return m_output;
}

void MyMagSen::setOutputSign(bool isSigned)
{
	if (ABS(m_output) == 1.0f)
		m_output = (isSigned)? -10.0f : 10.0f;
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

array<float, 2> &MyMagSen::getStandardizedValue(void)
{
	return m_standardizedReading;
}
