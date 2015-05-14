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

using namespace libsc;

class MySmartCar
{

public:

	explicit MySmartCar(void);

	MyResource		m_res;
	MyLcd			m_lcdConsole;
	MyLoop			m_loop;
	MyVarManager	m_varMng;
	MyServo			m_servo;
	MyMotor			m_motor;
	Led				m_led;
	MyBuzzer		m_buzzer;

private:

	static void showValue(const uint32_t &timeDelay);

};
