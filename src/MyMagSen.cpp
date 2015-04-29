/*
 * MyMagSen.cpp
 *
 * Author: Peter
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include <libbase/k60/pin.h>
#include <libbase/k60/adc.h>
#include "MyMagSen.h"
#include "MyResource.h"

using namespace libbase::k60;

inline Pin::Name getPin(const uint8_t id)
{
	switch (id)
	{
	default:
		assert(false);
		// no break
#ifdef LIBSC_USE_MAGSEN
	case 0:
		return LIBSC_MAGSEN4;
#endif

#if LIBSC_USE_MAGSEN > 1
	case 1:
		return LIBSC_MAGSEN5;
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
	config.pin = getPin((uint8_t)type + (uint8_t)which);
	return config;
}

MyMagSen::MyMagSen(MagSen type)
:
		m_magSen({ Adc(getAdcConfig(type, Side::LEFT)), Adc(getAdcConfig(type, Side::RIGHT)) })
{

}

