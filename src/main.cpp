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

	for (char c = 32; c < 32 + 21; c++)
	{
		myCar.m_lcdConsole << c;
		System::DelayMs(50);
	}
	myCar.m_lcdConsole << MyLcd::endl;
	for (char c = 32 + 21; c < 32 + 21 * 2; c++)
	{
//		if (c != 58)
		myCar.m_lcdConsole << c;
		System::DelayMs(50);
	}
	myCar.m_lcdConsole << MyLcd::endl;
	for (char c = 32 + 21 * 2; c < 32 + 21 * 3; c++)
	{
//		if (c != 58)
		myCar.m_lcdConsole << c;
		System::DelayMs(50);
	}
	myCar.m_lcdConsole.setRow(14) << "REF:00500 P:0.00015\nI:0.0005 D:0.0001";

	while (true);

}
