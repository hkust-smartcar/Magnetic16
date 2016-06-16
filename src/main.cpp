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

	//	pResource::m_instance->setMotorsEnabled(true);

	DelayMsByTicks(500);

	//	for (uint8_t i = 0; i < 400; i += 2)
	//	{
	//		pResource::m_instance->setMotorPower(0, i);
	//		pResource::m_instance->setMotorPower(1, i);
	//		DelayMsByTicks(500);
	//		pResource::m_instance->updateMotors();
	//		pResource::m_instance->sendDataToGrapher();
	//	}

	//	pResource::m_instance->setMotorPower(0, 0);
	//	pResource::m_instance->setMotorPower(1, 0);
	//	while (true);
	pResource::m_instance->testing();
//	pResource::m_instance->run();

}
