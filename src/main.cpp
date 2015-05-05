/*
 * main.cpp
 *
 * Author: Peter
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
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

MySmartCar myCar;

void myListener(const std::vector<Byte>& bytes)
{
	switch (bytes[0])
	{
	case 'a':
		MyResource::smartCar().m_motor.setEnabled(true);
		break;

	case 's':
		MyResource::smartCar().m_motor.setEnabled(false);
		break;
	}
}

int main(void)
{
	System::Init();


//	myCar.m_varMng.addWatchedVar((float *)&(myCar.m_servo.m_MagSen[0].getRawValue()[0]));
//	myCar.m_varMng.addWatchedVar((float *)&(myCar.m_servo.m_MagSen[0].getRawValue()[1]));

	myCar.m_varMng.addWatchedVar(myCar.m_motor.m_encoder.getEncoderCountPointer());
	myCar.m_varMng.addWatchedVar(myCar.m_motor.getSpeed());
	myCar.m_varMng.addWatchedVar(&MyResource::ConfigTable::MotorConfig::Reference);
	myCar.m_varMng.addWatchedVar(myCar.m_motor.m_speedPID.getLastError());

	myCar.m_varMng.addSharedVar(&MyResource::ConfigTable::MotorConfig::Kp, "Kp");
	myCar.m_varMng.addSharedVar(&MyResource::ConfigTable::MotorConfig::Ki, "Ki");
	myCar.m_varMng.addSharedVar(&MyResource::ConfigTable::MotorConfig::Kd, "Kd");
	myCar.m_varMng.addSharedVar(&MyResource::ConfigTable::MotorConfig::Reference, "Ref");

	myCar.m_varMng.Init(&myListener);

	myCar.m_loop.start();

}
