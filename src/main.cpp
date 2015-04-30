/*
 * main.cpp
 *
 *  Created on: Apr 22, 2015
 *      Author: Peter
 */

#include <cstring>
#include <libbase/k60/mcg.h>
#include <libsc/us_100.h>
#include <libsc/system.h>
#include "MySmartCar.h"

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

using namespace libsc;

int main(void)
{
	System::Init();

	MySmartCar myCar;

	myCar.m_lcdConsole.setRow(0);

	float i = 55.5;
	int16_t j = 120;

	myCar.m_varMng.Init();
	myCar.m_varMng.addWatchedVar(&i);
	myCar.m_varMng.addSharedVar(&i, "Lincoln");
	myCar.m_varMng.addWatchedVar(&j);
	myCar.m_varMng.addSharedVar(&j, "Peter");
	myCar.m_varMng.addSharedVar(&j, "Peter2");
	myCar.m_varMng.addSharedVar(&j, "Peter3");
	myCar.m_varMng.addSharedVar(&j, "Peter4");
	myCar.m_varMng.addSharedVar(&j, "Peter5");
	myCar.m_varMng.addSharedVar(&j, "Peter6");
	myCar.m_varMng.addSharedVar(&j, "Peter5");

	while (true)
	{
		myCar.m_varMng.sendWatchData();
		System::DelayMs(10);
	}

}
