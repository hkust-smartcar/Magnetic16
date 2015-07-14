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
#include "MyLcd.h"
#include "MySmartCar.h"
#include <libbase/k60/flash.h>

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

	case 'b':
		MyResource::smartCar().m_buzzer.setEnabled(!MyResource::smartCar().m_buzzer.isEnabled());
		break;

	case 'r':
		MyResource::smartCar().m_lcdConsole.onDraw(99999);
		break;

//	case 'a':
//		MyResource::ConfigTable::MotorConfig::Kp += 0.005f;
//		break;
//
//	case 'A':
//		MyResource::ConfigTable::ServoConfig::NormalKp += 10.0f;
//		break;
//
//	case 'z':
//		MyResource::ConfigTable::MotorConfig::Kp = (MyResource::ConfigTable::MotorConfig::Kp - 0.005f < 0.0f)? 0.0f : MyResource::ConfigTable::MotorConfig::Kp - 0.005f;
//		break;
//
//	case 'Z':
//		MyResource::ConfigTable::ServoConfig::NormalKp = (MyResource::ConfigTable::ServoConfig::NormalKp - 10.0f < 0.0f)? 0.0f : MyResource::ConfigTable::ServoConfig::NormalKp - 10.0f;
//		break;
//
//	case 's':
//		MyResource::ConfigTable::MotorConfig::Ki += 0.001f;
//		break;
//
//	case 'S':
//		MyResource::ConfigTable::ServoConfig::NormalKi += 0.1f;
//		break;
//
//	case 'x':
//		MyResource::ConfigTable::MotorConfig::Ki = (MyResource::ConfigTable::MotorConfig::Ki - 0.001f < 0.0f)? 0.0f : MyResource::ConfigTable::MotorConfig::Ki - 0.001f;
//		break;
//
//	case 'X':
//		MyResource::ConfigTable::ServoConfig::NormalKi = (MyResource::ConfigTable::ServoConfig::NormalKi - 0.1f < 0.0f)? 0.0f : MyResource::ConfigTable::ServoConfig::NormalKi - 0.1f;
//		break;
//
//	case 'd':
//		MyResource::ConfigTable::MotorConfig::Kd += 0.0001f;
//		break;
//
//	case 'D':
//		MyResource::ConfigTable::ServoConfig::NormalKd += 1.0f;
//		break;
//
//	case 'c':
//		MyResource::ConfigTable::MotorConfig::Kd = (MyResource::ConfigTable::MotorConfig::Kd - 0.0001f < 0.0f)? 0.0f : MyResource::ConfigTable::MotorConfig::Kd - 0.0001f;
//		break;
//
//	case 'C':
//		MyResource::ConfigTable::ServoConfig::NormalKd = (MyResource::ConfigTable::ServoConfig::NormalKd - 1.0f < 0.0f)? 0.0f : MyResource::ConfigTable::ServoConfig::NormalKd - 1.0f;
//		break;
//
	case 'f':
		MyResource::ConfigTable::MotorConfig::Reference += 100.0f;
		break;

	case 'v':
		MyResource::ConfigTable::MotorConfig::Reference = (MyResource::ConfigTable::MotorConfig::Reference - 100.0f < 0.0f)? 0.0f : MyResource::ConfigTable::MotorConfig::Reference - 100.0f;
		break;
//
//	case 'F':
//		MyResource::ConfigTable::ServoConfig::TurningKpA += 10.0f;
//		break;
//
//	case 'V':
//		MyResource::ConfigTable::ServoConfig::TurningKpA = (MyResource::ConfigTable::ServoConfig::TurningKpA - 10.0f < 0.0f)? 0.0f : MyResource::ConfigTable::ServoConfig::TurningKpA - 10.0f;
//		break;
	}

//	if (!MyResource::smartCar().m_mode)
//		MyResource::smartCar().m_lcdConsole.onDraw(99999);
}

void OnPress(const uint8_t id)
{
	switch (id)
	{
	default:
		assert(false);
		break;

	case 0:
//		myCar.m_batteryMeter.checkBattery(0);
		MyResource::smartCar().m_motor.setEnabled(!MyResource::smartCar().m_motor.isEnabled());
		break;

	case 1:
//		myCar.m_servo.setDegree(MID_SERVO_ANGLE);
//		myCar.m_servo.setEnabled(!myCar.m_servo.isEnabled());
//		myCar.m_motor.setEnabled(!myCar.m_motor.isEnabled());
		MyResource::smartCar().m_flash.eraseAll();
		break;
	}
}

int main(void)
{
	System::Init();

	myCar.m_varMng.addWatchedVar(&myCar.m_servo.m_MagSen[0].getOutputValue(), "SD_Output");
	myCar.m_varMng.addWatchedVar(&myCar.m_servo.m_MagSen[1].getOutputValue(), "FD_Output");
	myCar.m_varMng.addWatchedVar(&myCar.m_servo.m_MagSen[2].getOutputValue(), "HD_Output");
	myCar.m_varMng.addWatchedVar(&myCar.m_servo.m_MagSen[0].getFilteredValueAvg(), "SD_Avg");
	myCar.m_varMng.addWatchedVar(&myCar.m_servo.m_MagSen[1].getFilteredValueAvg(), "FD_Avg");
	myCar.m_varMng.addWatchedVar(&myCar.m_servo.m_MagSen[2].getFilteredValueAvg(), "HD_Avg");
//	myCar.m_varMng.addWatchedVar((uint8_t *)&MyResource::smartCar().m_servo.m_90DegreeTurningNeed, "90DegreeMode");
//	myCar.m_varMng.addWatchedVar((uint8_t *)&MyResource::smartCar().m_servo.m_isPidNonLinear, "PidNonLinear");

//	myCar.m_varMng.addWatchedVar(&myCar.m_servo.m_lastError, "IntegratedOutput");
//	myCar.m_varMng.addWatchedVar(myCar.m_servo.m_servoPid[MyServo::Normal].getLastError(), "LastError");
	myCar.m_varMng.addWatchedVar(&myCar.m_servo.m_finalAngle, "Angle");

//	myCar.m_varMng.addWatchedVar(&myCar.m_servo.m_MagSen[0].getFilteredValue()[0], "SD_FilteredValueL");
//	myCar.m_varMng.addWatchedVar(&myCar.m_servo.m_MagSen[0].getFilteredValue()[1], "SD_FilteredValueR");
//	myCar.m_varMng.addWatchedVar(&myCar.m_servo.m_MagSen[1].getFilteredValue()[0], "FD_FilteredValueL");
//	myCar.m_varMng.addWatchedVar(&myCar.m_servo.m_MagSen[1].getFilteredValue()[1], "FD_FilteredValueR");
//	myCar.m_varMng.addWatchedVar(&myCar.m_servo.m_MagSen[2].getFilteredValue()[0], "HD_FValueL");
//	myCar.m_varMng.addWatchedVar(&myCar.m_servo.m_MagSen[2].getFilteredValue()[1], "HD_FValueR");

//	myCar.m_varMng.addWatchedVar(myCar.m_motor.m_encoder.getEncoderCountPointer(), "EncoderCount");
//	myCar.m_varMng.addWatchedVar(&myCar.m_motor.getSpeed(), "Speed");
//	myCar.m_varMng.addWatchedVar(&MyResource::smartCar().m_motor.m_curReference, "EncoderTaget");
//	myCar.m_varMng.addWatchedVar((uint8_t *)&MyResource::smartCar().m_servo.m_lastTurningDirection, "LastDirection");
//	myCar.m_varMng.addWatchedVar(myCar.m_motor.m_speedPid.getLastError(), "Error");

	myCar.m_varMng.addSharedVar(&MyResource::ConfigTable::MotorConfig::Reference, "Motor_Ref");
	myCar.m_varMng.addSharedVar(&MyResource::ConfigTable::ServoConfig::NormalKp, "Servo_nKp");
	myCar.m_varMng.addSharedVar(&MyResource::ConfigTable::ServoConfig::NormalKd, "Servo_nKd");
	myCar.m_varMng.addSharedVar(&MyResource::ConfigTable::ServoConfig::TurningKpA, "Servo_tKpA");
	myCar.m_varMng.addSharedVar(&MyResource::ConfigTable::ServoConfig::TurningKpB, "Servo_tKpB");
	myCar.m_varMng.addSharedVar(&MyResource::ConfigTable::ServoConfig::TurningKd, "Servo_tKd");
//	myCar.m_varMng.addSharedVar(&MyResource::ConfigTable::ServoConfig::WeightSD, "Weight_SD");
//	myCar.m_varMng.addSharedVar(&MyResource::ConfigTable::ServoConfig::WeightFD, "Weight_FD");
//	myCar.m_varMng.addSharedVar(&MyResource::ConfigTable::ServoConfig::WeightHD, "Weight_HD");
//	myCar.m_varMng.addSharedVar(&MyResource::ConfigTable::MotorConfig::Kp, "Kp");
//	myCar.m_varMng.addSharedVar(&MyResource::ConfigTable::MotorConfig::Kd, "Kd");
	myCar.m_varMng.addSharedVar(&MyResource::ConfigTable::ServoConfig::SdNoSignalThreshold, "Sd_TurningTH");
//	myCar.m_varMng.addSharedVar(&MyResource::ConfigTable::ServoConfig::TurningThresholdHdValue, "Hd_TH");
//	myCar.m_varMng.addSharedVar(&MyResource::ConfigTable::MagSenConfig::Kq, "Kq");
//	myCar.m_varMng.addSharedVar(&MyResource::ConfigTable::MagSenConfig::Kr, "Kr");

	Button::Config buttonConfig;
	buttonConfig.listener_trigger = Button::Config::Trigger::kDown;
	buttonConfig.listener = &OnPress;

	buttonConfig.id = 0;
	Button bnt0(buttonConfig);

	buttonConfig.id = 1;
	Button bnt1(buttonConfig);

	myCar.m_varMng.SetOnReceiveListener(&myListener);
//	myCar.m_varMng.Init(&myListener);

	MyResource::smartCar().m_lcdConsole.onDraw(99999);

	myCar.m_batteryMeter.checkBattery(0);

	System::DelayMs(500);

	myCar.m_loop.start();

}
