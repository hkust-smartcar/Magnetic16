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
		config.pin = LIBSC_SWITCH6; break; // NewCar: gged

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
	m_switch(),
	m_mode(Mode::StandardDebug)
{
	switchInit();
	m_loop.addFunctionToLoop(&m_motor.updateSpeed, MyResource::ConfigTable::MotorConfig::UpdateFreq);
	m_loop.addFunctionToLoop(&m_servo.updateAngle, MyResource::ConfigTable::ServoConfig::UpdateFreq);
	m_loop.addFunctionToLoop(&showValue, MyResource::ConfigTable::VarMngConfig::UpdateFreq);
	m_loop.addFunctionToLoop(&m_lcdConsole.onDraw, MyResource::ConfigTable::LcdConfig::UpdateFreq);
//	m_loop.addFunctionToLoop(&m_batteryMeter.checkBattery, MyResource::ConfigTable::BatteryMeterConfig::UpdateFreq);
}

void MySmartCar::switchInit(void)
{
	for (int i = 0; i < 8; i++)
		m_switch[i] = Gpi(getSwitchConfig(i));
	m_mode = (Mode)(!m_switch[7].Get());
	m_servo.setEnabled(!m_switch[4].Get());
	m_servo.m_allow90DegreeTurning = m_switch[3].Get();
	MyResource::smartCar().m_mode = (Mode)(1 + (uint8_t)m_switch[5].Get());
}

void MySmartCar::switchOnTriggered(Gpi *target)
{
	switch (target->GetPin()->GetName())
	{
	case LIBSC_SWITCH7:
		MyResource::smartCar().m_lcdConsole.m_lcd->Clear();
		MyResource::smartCar().m_lcdConsole.setEnabled(true);

		if (target->Get())
			MyResource::smartCar().m_mode = Mode::Runtime;
		else
			MyResource::smartCar().m_mode = Mode::StandardDebug;

		MyResource::smartCar().m_lcdConsole.onDraw(99999);
		break;

	case LIBSC_SWITCH5:
		if (target->Get())
			MyResource::smartCar().m_mode = Mode::StandardDebug;
		else
			MyResource::smartCar().m_mode = Mode::RawDebug;
		break;

	case LIBSC_SWITCH4:
		MyResource::smartCar().m_servo.setEnabled(!target->Get());
		break;

	case LIBSC_SWITCH3:
		MyResource::smartCar().m_servo.m_allow90DegreeTurning = target->Get();
		break;
	}
}

void MySmartCar::showValue(const uint32_t &)
{
	MyResource::smartCar().m_varMng.sendWatchData();
}
