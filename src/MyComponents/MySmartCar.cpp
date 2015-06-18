/*
 * MySmartCar.cpp
 *
 * Author: Peter
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include <libsc/led.h>
#include <libbase/k60/gpio.h>
#include "MyServo.h"
#include "MyMotor.h"
#include "MyLcd.h"
#include "MyResource.h"
#include "MySmartCar.h"
#include "MyBuzzer.h"

using namespace libsc;

Gpi::Config MySmartCar::getSwitchConfig(const uint8_t id)
{
	Gpi::Config config;
	switch (id)
	{
	case 0:
		config.pin = LIBSC_SWITCH0; break;

	case 1:
		config.pin = LIBSC_SWITCH1; break;

	case 2:
		config.pin = LIBSC_SWITCH2; break;

	case 3:
		config.pin = LIBSC_SWITCH3; break;

	case 4:
		config.pin = LIBSC_SWITCH4; break;

	case 5:
		config.pin = LIBSC_SWITCH5; break;

	case 6:
		config.pin = LIBSC_SWITCH6; break;

	case 7:
		config.pin = LIBSC_SWITCH7; break;
	}
	config.interrupt = Pin::Config::Interrupt::kBoth;
	config.isr = &switchOnTriggered;
	return config;
}

MySmartCar::MySmartCar(void)
:
	m_res(this),
	m_lcdConsole(),
	m_loop(),
	m_varMng(),
	m_servo(),
	m_motor(),
	m_buzzer(),
	m_batteryMeter(),
	m_switch7(getSwitchConfig(7)),
	m_mode((Mode)!MyResource::smartCar().m_switch7.Get())
{
	m_loop.addFunctionToLoop(&m_motor.updateSpeed, MyResource::ConfigTable::MotorConfig::UpdateFreq);
	m_loop.addFunctionToLoop(&m_servo.updateAngle, MyResource::ConfigTable::ServoConfig::UpdateFreq);
	m_loop.addFunctionToLoop(&showValue, MyResource::ConfigTable::VarMngConfig::UpdateFreq);
	m_loop.addFunctionToLoop(&m_lcdConsole.onDraw, MyResource::ConfigTable::LcdConfig::UpdateFreq);
//	m_loop.addFunctionToLoop(&m_batteryMeter.checkBattery, MyResource::ConfigTable::BatteryMeterConfig::UpdateFreq);
}

void MySmartCar::switchOnTriggered(Gpi *)
{
	MyResource::smartCar().m_lcdConsole.m_lcd->Clear();

	if (MyResource::smartCar().m_switch7.Get())
		MyResource::smartCar().m_mode = Mode::Runtime;
	else
		MyResource::smartCar().m_mode = Mode::Debug;

	MyResource::smartCar().m_lcdConsole.onDraw(99999);
}

void MySmartCar::showValue(const uint32_t &)
{
	MyResource::smartCar().m_varMng.sendWatchData();
}
