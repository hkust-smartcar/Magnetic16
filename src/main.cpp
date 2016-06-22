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
#include <pBuzzer.h>
#include <pLoop.h>

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

int main(void)
{
	System::Init();

	pResource resource;

	resource.setMotorsEnabled(true);
//	resource.setMotorPower(0, 200);
//	while (true)
//	{
//		resource.updateMotors();
//		resource.sendDataToGrapher();
//		DelayMsByTicks(10);
//	}

	DelayMsByTicks(5000);

	for (int16_t i = 0; i <= 10000; i += 100)
	{
		resource.setMotorPower(0, i);
//		resource.setMotorPower(1, i);
		DelayMsByTicks(100);
		resource.updateMotors();
		resource.sendDataToGrapher();
	}

	pResource::m_instance->run();

}
