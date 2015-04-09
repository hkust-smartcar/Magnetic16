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

using namespace libsc;
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
		globalConfig.MyMotorSpeedControlRef += 10;
		break;
	case 'M':
		globalConfig.MyMotorSpeedControlRef = max(globalConfig.MyMotorSpeedControlRef - 10, 0.0f);
		break;

	case 'p':
		globalConfig.MyMotorSpeedControlKp += 0.05f;
		break;
	case 'P':
		globalConfig.MyMotorSpeedControlKp = max(globalConfig.MyMotorSpeedControlKp - 0.05f, 0.0f);
		break;

	case 'i':
		globalConfig.MyMotorSpeedControlKi += 0.0001f;
		break;
	case 'I':
		globalConfig.MyMotorSpeedControlKi = max(globalConfig.MyMotorSpeedControlKi - 0.0001f, 0.0f);
		break;

	case 'd':
		globalConfig.MyMotorSpeedControlKd += 0.1f;
		break;
	case 'D':
		globalConfig.MyMotorSpeedControlKd = max(globalConfig.MyMotorSpeedControlKd - 0.1f, 0.0f);
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

	case 'h':
		globalConfig.MyServoTurningKp += 10.0f;
		break;
	case 'H':
		globalConfig.MyServoTurningKp = max(globalConfig.MyServoTurningKp - 10.0f, 0.0f);
		break;
	case 'j':
		globalConfig.MyServoTurningKd += 10.0f;
		break;
	case 'J':
		globalConfig.MyServoTurningKd = max(globalConfig.MyServoTurningKd - 10.0f, 0.0f);
		break;

	case 'k':
		globalConfig.MyMagSenFilterQ += 0.0001f;
		break;
	case 'K':
		globalConfig.MyMagSenFilterQ = max(globalConfig.MyMagSenFilterQ - 0.0001f, 0.0f);
		break;
	case 'l':
		globalConfig.MyMagSenFilterR += 0.05f;
		break;
	case 'L':
		globalConfig.MyMagSenFilterR = max(globalConfig.MyMagSenFilterR - 0.05f, 0.0f);
		break;

	case ';':
		myCar.motorSetEnabled(true);
		break;
	case ':':
		myCar.motorSetEnabled(false);
		break;
	case '\'':
		myCar.servoSetEnabled(true);
		break;
	case '\"':
		myCar.servoSetEnabled(false);
		break;
	case ',':
		globalConfig.MySmartCarPowerMode = MyConfig::SmartCarPowerMode::kNormalMode;
		break;
	case '<':
		globalConfig.MySmartCarPowerMode = MyConfig::SmartCarPowerMode::kLowFrictionMode;
		break;
	}
}

int main()
{
//	myCar.myVarMng.addWatchedVar(globalVars.MagSenSDLeft, "asd");
//	myCar.myVarMng.addWatchedVar(globalVars.MagSenSDRight, "asd");
//	myCar.myVarMng.addWatchedVar(globalVars.MagSenSDRawLeft, "asd");
//	myCar.myVarMng.addWatchedVar(globalVars.MagSenSDRawRight, "asd");
//	myCar.myVarMng.addWatchedVar(globalVars.MagSenFDLeft, "asd");
//	myCar.myVarMng.addWatchedVar(globalVars.MagSenFDRight, "asd");
//	myCar.myVarMng.addWatchedVar(globalVars.MagSenHDLeft, "asd");
//	myCar.myVarMng.addWatchedVar(globalVars.MagSenHDRight, "asd");
//	myCar.myVarMng.addWatchedVar(globalVars.magSenReferenceReading, "asd");
//	myCar.myVarMng.addWatchedVar(globalVars.lastAngleListSum, "asd");
//	myCar.myVarMng.addWatchedVar(globalVars.MagSenFDRight, "asd");
//	myCar.myVarMng.addWatchedVar(globalVars.lastTurningAngle, "asd");
//	myCar.myVarMng.addWatchedVar(&globalConfig.MyMagSenFilterQ, "ASd");
//	myCar.myVarMng.addWatchedVar(&globalConfig.MyMagSenFilterR, "ASF");
//	myCar.myVarMng.addWatchedVar(globalVars.speed, "asd");
	myCar.myVarMng.addWatchedVar(&globalConfig.MyMotorSpeedControlKp, "asd");
	myCar.myVarMng.addWatchedVar(&globalConfig.MyMotorSpeedControlKi, "asd");
	myCar.myVarMng.addWatchedVar(&globalConfig.MyMotorSpeedControlKd, "asd");
	myCar.myVarMng.addWatchedVar(globalVars.lastCount, "asd");
	myCar.myVarMng.addWatchedVar(&globalConfig.MyMotorSpeedControlRef, "asd");
	myCar.myVarMng.Init(&myListener);

	myCar.startMainLoop();

}
