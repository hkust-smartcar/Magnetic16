/*
 * MySmartCar.h
 *
 *  Created on: Apr 22, 2015
 *      Author: Peter
 */

#pragma once

#include <libsc/system.h>
#include <libsc/led.h>
#include "MyLcd.h"
#include "MyServo.h"
#include "MyMotor.h"
#include "MyBuzzer.h"
#include "MyBuzzer.h"
#include "MyResource.h"
#include "MyLoop.h"
#include "MyVarManager.h"
#include "MyBatteryMeter.h"
#include "MyMenu.h"
#include "MyFlash.h"

#define IS_FRONT 1

using namespace libsc;

class MySmartCar
{

public:

	enum Mode
	{
		Runtime = 0,
		RawDebug,
		StandardDebug
	};

	typedef Gpi		MySwitch;

	explicit MySmartCar(void);

	Mode			m_mode;

	MyResource		m_res;
	MyFlash			m_flash;
	MyLcd			m_lcdConsole;
	MyLoop			m_loop;
	MyVarManager	m_varMng;
	MyServo			m_servo;
	MyMotor			m_motor;
	MyBuzzer		m_buzzer;
	MyBatteryMeter	m_batteryMeter;
	array<MySwitch, 8> m_switch;
	MyHallSensor	m_hallSensor;
//	MyMenu			m_menu;


private:

	MySwitch::Config getSwitchConfig(const uint8_t id);

	void switchInit(void);
	static void switchOnTriggered(Gpi *target);
	static void showValue(const uint32_t &timeDelay);

};
