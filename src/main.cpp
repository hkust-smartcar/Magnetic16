/*
 * main.cpp
 *
 *  Created on: Dec 29, 2014
 *      Author: Peter
 */

#include <libbase/k60/mcg.h>

#include "MySmartCar.h"
#include "PIDhandler.h"
#include "MyConfig.h"
#include "MyVarManager.h"
#include "MyVar.h"
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

using namespace libsc::k60;
using namespace libbase::k60;
using namespace std;

MyConfig globalConfig;
MyVar globalVars;
MySmartCar myCar(globalConfig, globalVars);

void myListener(const vector<Byte> &bytes)
{
	switch (bytes[0])
	{

	case 'm':
		globalConfig.MyMotorSpeedControlRef += 50;
		break;
	case 'M':
		globalConfig.MyMotorSpeedControlRef = max(globalConfig.MyMotorSpeedControlRef - 50, 0.0f);
		break;

	case 'p':
		globalConfig.MyMotorSpeedControlKp += 0.00005f;
		break;
	case 'P':
		globalConfig.MyMotorSpeedControlKp = max(globalConfig.MyMotorSpeedControlKp - 0.00005f, 0.0f);
		break;

	case 'i':
		globalConfig.MyMotorSpeedControlKi += 0.00005f;
		break;
	case 'I':
		globalConfig.MyMotorSpeedControlKi = max(globalConfig.MyMotorSpeedControlKi - 0.00005f, 0.0f);
		break;

	case 'q':
		globalConfig.MyMagSenFilterQ += 0.0005f;
		break;
	case 'Q':
		globalConfig.MyMagSenFilterQ = max(globalConfig.MyMagSenFilterQ - 0.0005f, 0.0f);
		break;
	case 'r':
		globalConfig.MyMagSenFilterR += 0.01f;
		break;
	case 'R':
		globalConfig.MyMagSenFilterR = max(globalConfig.MyMagSenFilterR - 0.01f, 0.0f);
		break;
	}
}

int main()
{
	myCar.myVarMng.addWatchedVar(globalVars.MagSenSDLeft, "asd");
	myCar.myVarMng.addWatchedVar(globalVars.MagSenSDRight, "asd");
	myCar.myVarMng.addWatchedVar(globalVars.MagSenFDLeft, "asd");
	myCar.myVarMng.addWatchedVar(globalVars.MagSenFDRight, "asd");
	myCar.myVarMng.addWatchedVar(globalVars.magSenDefaultMaxFDValue, "asd");
	myCar.myVarMng.addWatchedVar(globalVars.lastTurningBeforeLowSignal, "asd");
//	myCar.myVarMng.addWatchedVar(globalVars.speed, "asd");
//	myCar.myVarMng.addWatchedVar(globalVars.lastCount, "asd");
	myCar.myVarMng.Init(&myListener);

	myCar.startMainLoop();
	vector<Led> myLeds;
	Gpo::Config config;
	config.is_high = true;
	config.pin = libbase::k60::Pin::Name::kPtc1;
	Gpio a(config);

	for (uint8_t i = 0; i < 4; i++)
		myLeds.push_back(Led({i, false}));

	while (true)
		for (uint8_t j = 0; j < 4; j++)
			myLeds[j].Switch();

}
