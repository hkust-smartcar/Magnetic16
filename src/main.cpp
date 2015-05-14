/*
 * main.cpp
 *
 * Author: Peter
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include <cstring>
#include <libbase/k60/mcg.h>
#include <libbase/k60/gpio.h>
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
using namespace libbase::k60;

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

	myCar.m_varMng.addWatchedVar(&myCar.m_servo.m_MagSen[0].getOutputValue(), "SD_Output");
//	myCar.m_varMng.addWatchedVar(&myCar.m_servo.m_MagSen[1].getOutputValue());
//	myCar.m_varMng.addWatchedVar(&myCar.m_servo.m_MagSen[2].getOutputValue());
//	myCar.m_varMng.addWatchedVar(&myCar.m_servo.m_lastAngle);

	myCar.m_varMng.addWatchedVar(&myCar.m_servo.m_MagSen[0].getFilteredValue()[0], "SD_FilteredValueL");
	myCar.m_varMng.addWatchedVar(&myCar.m_servo.m_MagSen[0].getFilteredValue()[1], "SD_FilteredValueR");
//	myCar.m_varMng.addWatchedVar(&myCar.m_servo.m_MagSen[1].getFilteredValue()[0]);
//	myCar.m_varMng.addWatchedVar(&myCar.m_servo.m_MagSen[1].getFilteredValue()[1]);
//	myCar.m_varMng.addWatchedVar(&myCar.m_servo.m_MagSen[2].getFilteredValue()[0]);
//	myCar.m_varMng.addWatchedVar(&myCar.m_servo.m_MagSen[2].getFilteredValue()[1]);

	myCar.m_varMng.addWatchedVar(myCar.m_motor.m_encoder.getEncoderCountPointer(), "EncoderCount");
//	myCar.m_varMng.addWatchedVar(myCar.m_motor.getSpeed());
//	myCar.m_varMng.addWatchedVar(&MyResource::ConfigTable::MotorConfig::Reference);
//	myCar.m_varMng.addWatchedVar(myCar.m_motor.m_speedPID.getLastError());

	myCar.m_varMng.addSharedVar(&MyResource::ConfigTable::ServoConfig::Kp, "Servo_Kp");
	myCar.m_varMng.addSharedVar(&MyResource::ConfigTable::ServoConfig::Ki, "Servo_Ki");
	myCar.m_varMng.addSharedVar(&MyResource::ConfigTable::ServoConfig::Kd, "Servo_Kd");
	myCar.m_varMng.addSharedVar(&MyResource::ConfigTable::MotorConfig::Reference, "Motor_Ref");
	myCar.m_varMng.addSharedVar(&MyResource::ConfigTable::ServoConfig::WeightSD, "Weight_SD");
	myCar.m_varMng.addSharedVar(&MyResource::ConfigTable::ServoConfig::WeightFD, "Weight_FD");
	myCar.m_varMng.addSharedVar(&MyResource::ConfigTable::ServoConfig::WeightHD, "Weight_HD");
//	myCar.m_varMng.addSharedVar(&MyResource::ConfigTable::MagSenConfig::Kq, "Kq");
//	myCar.m_varMng.addSharedVar(&MyResource::ConfigTable::MagSenConfig::Kr, "Kr");

	myCar.m_varMng.Init(&myListener);

	myCar.m_loop.start();

}
