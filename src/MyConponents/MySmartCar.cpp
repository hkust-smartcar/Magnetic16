/*
 * MySmarCar.cpp
 *
 *  Created on: Apr 25, 2015
 *      Author: Peter
 */

#include <libsc/led.h>
#include "MyServo.h"
#include "MyMotor.h"
#include "MyLcd.h"
#include "MySmartCar.h"
#include "MyResource.h"
#include "MyBuzzer.h"

using namespace libsc;

MySmartCar::MySmartCar(void)
:
	m_res(this),
	m_loop(),
	m_varMng(),
	m_servo(),
	m_motor(),
	m_led({0})
//	m_buzzer()
{
	m_loop.addFunctionToLoop(&m_motor.speedControlRoutine, MyResource::ConfigTable::MotorConfig::UpdateFreq);
	m_loop.addFunctionToLoop(&m_servo.servoAngleRoutine, MyResource::ConfigTable::MotorConfig::UpdateFreq);
	m_loop.addFunctionToLoop(&showValue, MyResource::ConfigTable::VarMngConfig::UpdateFreq);
	m_loop.addFunctionToLoop(&m_lcdConsole.onDraw, MyResource::ConfigTable::LcdConfig::UpdateFreq);
}

void MySmartCar::showValue(const uint32_t &timeDelay)
{
	MyResource::smartCar().m_varMng.sendWatchData();
}
