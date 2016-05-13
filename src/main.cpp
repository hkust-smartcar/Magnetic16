/*
 * main.cpp
 *
 * Author: Peter
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include <cassert>
#include <cstring>
#include <array>
#include <libbase/k60/mcg.h>
#include <libsc/button.h>
#include <libsc/joystick.h>
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

using namespace std;
using namespace libsc;
using namespace libbase::k60;

//MySmartCar myCar;

//void myListener(const std::vector<Byte>& bytes)
//{
//	switch (bytes[0])
//	{
//	case 'e':
//		MyResource::smartCar().m_motor.setEnabled(!MyResource::smartCar().m_motor.isEnabled());
//		break;
//
//	case 'E':
//		MyResource::smartCar().m_servo.setEnabled(!MyResource::smartCar().m_servo.isEnabled());
//		break;
//
//	case 'b':
//		MyResource::smartCar().m_buzzer.setEnabled(!MyResource::smartCar().m_buzzer.isEnabled());
//		break;
//
//	case 'r':
//		MyResource::smartCar().m_lcdConsole.onDraw(99999);
//		break;
//
//	case 'w':
//		MyResource::smartCar().m_motor.setSpeed(400);
//		MyResource::smartCar().m_motor.setEnabled(true);
//		break;
//
//	case 's':
//		MyResource::smartCar().m_motor.setSpeed(-100);
//		MyResource::smartCar().m_motor.setEnabled(false);
//		break;
//
//	case 'a':
//		MyResource::smartCar().m_servo.setEnabled(true);
//		MyResource::smartCar().m_servo.setAngle(450);
//		break;
//
//	case 'd':
//		MyResource::smartCar().m_servo.setEnabled(true);
//		MyResource::smartCar().m_servo.setAngle(-450);
//		break;
//
////	case 'a':
////		MyResource::ConfigTable::MotorConfig::Kp += 0.005f;
////		break;
////
////	case 'A':
////		MyResource::ConfigTable::ServoConfig::NormalKp += 10.0f;
////		break;
////
////	case 'z':
////		MyResource::ConfigTable::MotorConfig::Kp = (MyResource::ConfigTable::MotorConfig::Kp - 0.005f < 0.0f)? 0.0f : MyResource::ConfigTable::MotorConfig::Kp - 0.005f;
////		break;
////
////	case 'Z':
////		MyResource::ConfigTable::ServoConfig::NormalKp = (MyResource::ConfigTable::ServoConfig::NormalKp - 10.0f < 0.0f)? 0.0f : MyResource::ConfigTable::ServoConfig::NormalKp - 10.0f;
////		break;
////
////	case 's':
////		MyResource::ConfigTable::MotorConfig::Ki += 0.001f;
////		break;
////
////	case 'S':
////		MyResource::ConfigTable::ServoConfig::NormalKi += 0.1f;
////		break;
////
////	case 'x':
////		MyResource::ConfigTable::MotorConfig::Ki = (MyResource::ConfigTable::MotorConfig::Ki - 0.001f < 0.0f)? 0.0f : MyResource::ConfigTable::MotorConfig::Ki - 0.001f;
////		break;
////
////	case 'X':
////		MyResource::ConfigTable::ServoConfig::NormalKi = (MyResource::ConfigTable::ServoConfig::NormalKi - 0.1f < 0.0f)? 0.0f : MyResource::ConfigTable::ServoConfig::NormalKi - 0.1f;
////		break;
////
////	case 'd':
////		MyResource::ConfigTable::MotorConfig::Kd += 0.0001f;
////		break;
////
////	case 'D':
////		MyResource::ConfigTable::ServoConfig::NormalKd += 1.0f;
////		break;
////
////	case 'c':
////		MyResource::ConfigTable::MotorConfig::Kd = (MyResource::ConfigTable::MotorConfig::Kd - 0.0001f < 0.0f)? 0.0f : MyResource::ConfigTable::MotorConfig::Kd - 0.0001f;
////		break;
////
////	case 'C':
////		MyResource::ConfigTable::ServoConfig::NormalKd = (MyResource::ConfigTable::ServoConfig::NormalKd - 1.0f < 0.0f)? 0.0f : MyResource::ConfigTable::ServoConfig::NormalKd - 1.0f;
////		break;
////
//	case 'f':
//		MyResource::ConfigTable::MotorConfig::Reference += 100.0f;
//		break;
//
//	case 'v':
//		MyResource::ConfigTable::MotorConfig::Reference = (MyResource::ConfigTable::MotorConfig::Reference - 100.0f < 0.0f)? 0.0f : MyResource::ConfigTable::MotorConfig::Reference - 100.0f;
//		break;
////
////	case 'F':
////		MyResource::ConfigTable::ServoConfig::TurningKpA += 10.0f;
////		break;
////
////	case 'V':
////		MyResource::ConfigTable::ServoConfig::TurningKpA = (MyResource::ConfigTable::ServoConfig::TurningKpA - 10.0f < 0.0f)? 0.0f : MyResource::ConfigTable::ServoConfig::TurningKpA - 10.0f;
////		break;
//	}
//
////	if (!MyResource::smartCar().m_mode)
////		MyResource::smartCar().m_lcdConsole.onDraw(99999);
//	myCar.m_lastTimeUpdateKey = System::Time();
//}
//
//void OnPress(const uint8_t id)
//{
//	switch (id)
//	{
//	default:
//		assert(false);
//		break;
//
//	case 0:
////		myCar.m_batteryMeter.checkBattery(0);
//		DelayMsByTicks(500);
//		MyResource::smartCar().m_motor.setEnabled(!MyResource::smartCar().m_motor.isEnabled());
//		break;
//
//	case 1:
////		myCar.m_servo.setDegree(MID_SERVO_ANGLE);
////		myCar.m_servo.setEnabled(!myCar.m_servo.isEnabled());
////		myCar.m_motor.setEnabled(!myCar.m_motor.isEnabled());
//		MyResource::smartCar().m_flash.eraseAll();
//		break;
//	}
//}

static void buttonDIspatcher(const uint8_t id)
{
	switch (id)
	{
	case 0:

		break;

	case 1:

		break;

	case 2:

		break;

	default:
		assert(false);
	}
}

int main(void)
{
	System::Init();

//	Button::Config buttonConfig;
//	buttonConfig.listener_trigger = Button::Config::Trigger::kDown;
//	buttonConfig.listener = &OnPress;
//
//	buttonConfig.id = 0;
//	Button bnt0(buttonConfig);
//
//	buttonConfig.id = 1;
//	Button bnt1(buttonConfig);
//
//	myCar.m_varMng.SetOnReceiveListener(&myListener);
////	myCar.m_varMng.Init(&myListener);
//
//	MyResource::smartCar().m_lcdConsole.onDraw(99999);
//
//	myCar.m_batteryMeter.checkBattery(0);
//
//	System::DelayMs(500);
//
////	myCar.m_servo.setAngle(0);
//
//	myCar.m_loop.start();

	array<Led, 4> leds{ Led({ 0, true }), Led({ 1, true }), Led({ 2, true }), Led({ 3, true }) };

	function<void (const uint8_t)> buttonsDispatcher = [&](const uint8_t id)
															{
																leds[id].Switch();
															};
	function<void (const uint8_t, const Joystick::State which)> joystickDispatcher = [&](const uint8_t id, const Joystick::State which)
															{
																if ((uint8_t)which + 1 < 5)
																	leds[(uint8_t)which].Switch();
																else
																{
																	leds[0].Switch();
																	leds[3].Switch();
																}
															};

	Button::Config butConfig;
	butConfig.id = 0; butConfig.is_active_low = true; butConfig.listener = buttonsDispatcher; butConfig.listener_trigger = Button::Config::Trigger::kBoth;
	Button but0(butConfig); butConfig.id++;
	Button but1(butConfig); butConfig.id++;
	Button but2(butConfig);

	Joystick::Config jsConfig({ 0, true, joystickDispatcher, { 0 }, { Joystick::Config::Trigger::kBoth, Joystick::Config::Trigger::kBoth, Joystick::Config::Trigger::kBoth, Joystick::Config::Trigger::kBoth, Joystick::Config::Trigger::kBoth } });
//	jsConfig.id = 0;
//	jsConfig.is_active_low = true;
//	jsConfig.listeners = (Joystick::Listener *)array<function<void (const uint8_t id)>, 5>{ joystickDispatcher, joystickDispatcher, joystickDispatcher, joystickDispatcher, joystickDispatcher }.data();
//	jsConfig.listener_triggers = (Joystick::Config::Trigger *)array<Joystick::Config::Trigger, 5>{ Joystick::Config::Trigger::kBoth, Joystick::Config::Trigger::kBoth, Joystick::Config::Trigger::kBoth, Joystick::Config::Trigger::kBoth, Joystick::Config::Trigger::kBoth }.data();
	Joystick joystick(jsConfig);

//	for (uint8_t i = 0; i < leds.size(); i++)
//		leds[i] = Led({ i, true });

//	uint8_t index = 0;

	while (true)
	{
//		leds[index].Switch();
//		index = ++index % leds.size();
//		System::DelayMs(100);
	}

}
