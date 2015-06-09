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
#include <libsc/button.h>
#include <libbase/k60/gpio.h>
#include <libsc/system.h>
#include "MyResource.h"
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
	case 'e':
		MyResource::smartCar().m_motor.setEnabled(!MyResource::smartCar().m_motor.isEnabled());
		break;

	case 'E':
		MyResource::smartCar().m_servo.setEnabled(!MyResource::smartCar().m_servo.isEnabled());
		break;

	case 'a':
		MyResource::ConfigTable::MotorConfig::Kp += 0.005f;
		break;

	case 'A':
		MyResource::ConfigTable::ServoConfig::Kp += 10.0f;
		break;

	case 'z':
		MyResource::ConfigTable::MotorConfig::Kp = (MyResource::ConfigTable::MotorConfig::Kp - 0.005f < 0.0f)? 0.0f : MyResource::ConfigTable::MotorConfig::Kp - 0.005f;
		break;

	case 'Z':
		MyResource::ConfigTable::ServoConfig::Kp = (MyResource::ConfigTable::ServoConfig::Kp - 10.0f < 0.0f)? 0.0f : MyResource::ConfigTable::ServoConfig::Kp - 10.0f;
		break;

	case 's':
		MyResource::ConfigTable::MotorConfig::Ki += 0.001f;
		break;

	case 'S':
		MyResource::ConfigTable::ServoConfig::Ki += 0.1f;
		break;

	case 'x':
		MyResource::ConfigTable::MotorConfig::Ki = (MyResource::ConfigTable::MotorConfig::Ki - 0.001f < 0.0f)? 0.0f : MyResource::ConfigTable::MotorConfig::Ki - 0.001f;
		break;

	case 'X':
		MyResource::ConfigTable::ServoConfig::Ki = (MyResource::ConfigTable::ServoConfig::Ki - 0.1f < 0.0f)? 0.0f : MyResource::ConfigTable::ServoConfig::Ki - 0.1f;
		break;

	case 'd':
		MyResource::ConfigTable::MotorConfig::Kd += 0.0001f;
		break;

	case 'D':
		MyResource::ConfigTable::ServoConfig::Kd += 1.0f;
		break;

	case 'c':
		MyResource::ConfigTable::MotorConfig::Kd = (MyResource::ConfigTable::MotorConfig::Kd - 0.0001f < 0.0f)? 0.0f : MyResource::ConfigTable::MotorConfig::Kd - 0.0001f;
		break;

	case 'C':
		MyResource::ConfigTable::ServoConfig::Kd = (MyResource::ConfigTable::ServoConfig::Kd - 1.0f < 0.0f)? 0.0f : MyResource::ConfigTable::ServoConfig::Kd - 1.0f;
		break;

	case 'f':
		MyResource::ConfigTable::MotorConfig::Reference += 100.0f;
		break;

	case 'v':
		MyResource::ConfigTable::MotorConfig::Reference = (MyResource::ConfigTable::MotorConfig::Reference - 100.0f < 0.0f)? 0.0f : MyResource::ConfigTable::MotorConfig::Reference - 100.0f;
		break;
	}

	MyResource::smartCar().m_lcdConsole.onDraw(0);
}

void OnPress(const uint8_t id)
{
	switch (id)
	{
	default:
		assert(false);
		break;

	case 0:
		myCar.m_batteryMeter.checkBattery(0);
		break;

	case 1:
		myCar.m_servo.setDegree(MID_SERVO_ANGLE);
		myCar.m_servo.setEnabled(!myCar.m_servo.isEnabled());
		break;
	}
}

int main(void)
{
	System::Init();

	myCar.m_varMng.addWatchedVar(&myCar.m_servo.m_MagSen[0].getOutputValue(), "SD_Output");
	myCar.m_varMng.addWatchedVar(&myCar.m_servo.m_MagSen[1].getOutputValue(), "FD_Output");
	myCar.m_varMng.addWatchedVar(&myCar.m_servo.m_MagSen[2].getOutputValue(), "HD_Output");
//	myCar.m_varMng.addWatchedVar(&myCar.m_servo.m_lastAngle, "IntegratedOutput");
	myCar.m_varMng.addWatchedVar(&myCar.m_servo.m_lastDegree, "Angle");

//	myCar.m_varMng.addWatchedVar(&myCar.m_servo.m_MagSen[0].getFilteredValue()[0], "SD_FilteredValueL");
//	myCar.m_varMng.addWatchedVar(&myCar.m_servo.m_MagSen[0].getFilteredValue()[1], "SD_FilteredValueR");
//	myCar.m_varMng.addWatchedVar(&myCar.m_servo.m_MagSen[1].getFilteredValue()[0], "FD_FilteredValueL");
//	myCar.m_varMng.addWatchedVar(&myCar.m_servo.m_MagSen[1].getFilteredValue()[1], "FD_FilteredValueR");
//	myCar.m_varMng.addWatchedVar(&myCar.m_servo.m_MagSen[2].getFilteredValue()[0], "HD_FilteredValueL");
//	myCar.m_varMng.addWatchedVar(&myCar.m_servo.m_MagSen[2].getFilteredValue()[1], "HD_FilteredValueR");

//	myCar.m_varMng.addWatchedVar(myCar.m_motor.m_encoder.getEncoderCountPointer(), "EncoderCount");
//	myCar.m_varMng.addWatchedVar(myCar.m_motor.getSpeed());
//	myCar.m_varMng.addWatchedVar(&MyResource::ConfigTable::MotorConfig::Reference, "EncoderTaget");
//	myCar.m_varMng.addWatchedVar(myCar.m_motor.m_speedPID.getLastError());

//	myCar.m_varMng.addSharedVar(&MyResource::ConfigTable::ServoConfig::Kp, "Servo_Kp");
//	myCar.m_varMng.addSharedVar(&MyResource::ConfigTable::ServoConfig::Ki, "Servo_Ki");
//	myCar.m_varMng.addSharedVar(&MyResource::ConfigTable::ServoConfig::Kd, "Servo_Kd");
//	myCar.m_varMng.addSharedVar(&MyResource::ConfigTable::MotorConfig::Kp, "Motor_Kp");
//	myCar.m_varMng.addSharedVar(&MyResource::ConfigTable::MotorConfig::Ki, "Motor_Ki");
//	myCar.m_varMng.addSharedVar(&MyResource::ConfigTable::MotorConfig::Kd, "Motor_Kd");
//	myCar.m_varMng.addSharedVar(&MyResource::ConfigTable::ServoConfig::WeightSD, "Weight_SD");
//	myCar.m_varMng.addSharedVar(&MyResource::ConfigTable::ServoConfig::WeightFD, "Weight_FD");
//	myCar.m_varMng.addSharedVar(&MyResource::ConfigTable::ServoConfig::WeightHD, "Weight_HD");
//	myCar.m_varMng.addSharedVar(&MyResource::ConfigTable::MotorConfig::Reference, "Motor_Ref");
//	myCar.m_varMng.addSharedVar(&MyResource::ConfigTable::MagSenConfig::Kq, "Kq");
//	myCar.m_varMng.addSharedVar(&MyResource::ConfigTable::MagSenConfig::Kr, "Kr");

	Button::Config buttonConfig;
	buttonConfig.listener_trigger = Button::Config::Trigger::kDown;
	buttonConfig.listener = &OnPress;

	buttonConfig.id = 0;
	Button bnt0(buttonConfig);

	buttonConfig.id = 1;
	Button bnt1(buttonConfig);

	myCar.m_varMng.Init(&myListener);

	MyResource::smartCar().m_lcdConsole.onDraw(0);

	myCar.m_batteryMeter.checkBattery(0);

	myCar.m_loop.start();

}
