/*
 * main.cpp
 *
 * Author: Peter
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include <cassert>
#include <cstring>
#include <libbase/k60/mcg.h>
#include <pResource.h>
#include <pSmartCar.h>
#include <pLoop.h>,

namespace libbase
{
	namespace k60
	{

		Mcg::Config Mcg::GetMcgConfig()
		{
			Mcg::Config config;
			config.external_oscillator_khz = 50000;
			config.core_clock_khz = 150000;
			return config;
		}

	}
}

using namespace std;
using namespace libsc;
using namespace libbase::k60;
using namespace libutil;

//MySmartCar myCar;

int main(void)
{
	System::Init();

	pResource resource;

	System::DelayMs(500);

	uint16_t time = System::Time();

//	for (uint16_t i = 0; i < 450; i += 5)
//	{
//		resource.m_lcd.setRow(0);
//		resource.m_lcd << i << '\t';
//		resource.m_motors[0].setPower(-i);
//		resource.m_motors[1].setPower(-i);
//		DelayMsByTicks(100);
//		resource.m_motors[0].update();
//		resource.m_motors[1].update();
//		resource.m_grapher.sendWatchData();
//		DelayMsByTicks(100);
//		resource.m_motors[0].update();
//		resource.m_motors[1].update();
//		resource.m_grapher.sendWatchData();
//	}
//
//	resource.m_motors[0].setPower(0);
//	resource.m_motors[1].setPower(0);
//
//	while (true);

	pResource::m_instance->run();

}
