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
#include <libutil/pGrapher.h>
#include <libsc/ldc1000.h>
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

//	resource.setMotorsEnabled(true);
//	resource.setMotorPower(0, 2000);
//	resource.setMotorPower(1, 2000);
//	while (true)
//	{
//		resource.updateMotors();
//		resource.sendDataToGrapher();
//		DelayMsByTicks(10);
//	}

	DelayMsByTicks(500);

//	for (int16_t i = 0; i >= -3000; i -= 10)
//	{
//		resource.setMotorPower(0, i);
//		resource.setMotorPower(1, i);
//		DelayMsByTicks(10);
//		resource.updateMotors();
//		resource.sendDataToGrapher();
//	}
//	resource.setMotorPower(0, 0);
//	resource.setMotorPower(1, 0);

	pResource::m_instance->run();

}
