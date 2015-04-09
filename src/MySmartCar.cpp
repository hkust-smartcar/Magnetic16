/*
 * mySmartCar.cpp
 *
 *  Created on: Dec 29, 2014
 *      Author: Peter
 */

#include <cstring>
#include <cassert>
#include <stdlib.h>

#include <libsc/config.h>
#include <libsc/system.h>

#include "MySmartCar.h"
#include "MyMotor.h"
#include "MyServo.h"
#include "MyConfig.h"
#include "MyVar.h"

using namespace libsc;
using namespace libbase::k60;
using namespace std;

#define	inRange(n, v, x) ((v > x)? x : ((v < n)? n : v))
#define outRangeOf(v, ov, r) ((v - ov < -r || v - ov > r)? v : 0)
#define abs(v) ((v > 0)? v : -v)

MySmartCar *m_smartCarInstance;

Byte getLedCount(void)
{
#if (LIBSC_USE_LED == 1)
	return 1;
#elif (LIBSC_USE_LED == 2)
	return 2;
#elif (LIBSC_USE_LED == 3)
	return 3;
#elif (LIBSC_USE_LED == 4)
	return 4;
#else
	assert(false);
#endif
}

Button::Config MySmartCar::getButtonConfig(const uint8_t id)
{
	Button::Config config;
	config.id = id;
	config.is_active_low = false;
	config.is_use_pull_resistor = false;
	config.listener = &onButtonPress;
	config.listener_trigger = Button::Config::Trigger::kDown;
	return config;
}

MySmartCar::MySmartCar(MyConfig &config, MyVar &vars)
:
	myLoop(),
	m_powerMode(&config.MySmartCarPowerMode),
	myButtons({Button(getButtonConfig(0)), Button(getButtonConfig(1))}),
	myLcd(config, vars, myLoop),
	myMotor(config, vars, myLoop),
	myServo(config, vars, myLoop),
	myConfig(&config),
	myVars(&vars)
{
	m_smartCarInstance = this;
	reset();
}

void MySmartCar::showValue(void)
{
//	m_smartCarInstance->myVarMng.sendWatchData();

	MyLcd::getMyLcdInstance()->setRow(1)
				 << "SLeft: " << *(m_smartCarInstance->myVars->MagSenSDLeft) << MyLcd::endl
				 << "SRight: " << *(m_smartCarInstance->myVars->MagSenSDRight) << MyLcd::endl
				 << "FLeft: " << *(m_smartCarInstance->myVars->MagSenFDLeft) << MyLcd::endl
				 << "FRight: " << *(m_smartCarInstance->myVars->MagSenFDRight) << MyLcd::endl
				 << "HLeft: " << *(m_smartCarInstance->myVars->MagSenHDLeft) << MyLcd::endl
				 << "HRight: " << *(m_smartCarInstance->myVars->MagSenHDRight) << MyLcd::endl
//				 << "kP:" << (uint16_t)(m_smartCarInstance->myConfig->MyServoTurningKp) << " kD:" << (uint16_t)(m_smartCarInstance->myConfig->MyServoTurningKd) << MyLcd::endl
// 				 << "kQ:" << (m_smartCarInstance->myConfig->MyMagSenFilterQ) << " kR:" << (m_smartCarInstance->myConfig->MyMagSenFilterR) << MyLcd::endl
//				 << "Angle: " << (int16_t)*(m_smartCarInstance->myVars->lastTurningAngle) << MyLcd::endl
				 << "Encoder: " << (int32_t)*(m_smartCarInstance->myVars->lastCount) << MyLcd::endl
//				 << "Speed: " << (int16_t)(m_smartCarInstance->myConfig->MyMotorSpeedControlRef) << MyLcd::endl
				 << "T:" << (uint8_t)*(m_smartCarInstance->myVars->TurningState) << " S:" << (uint8_t)*(m_smartCarInstance->myVars->CurrentStatus) << " P:" << (int8_t)*(m_smartCarInstance->myVars->PositionState) << " ";
}

void MySmartCar::startMainLoop(void)
{
	myLoop.addFunctionToLoop(&showValue, 1, 150);
//	myLoop.addFunctionToLoop(&showValue, 1, 20);
	myLoop.start();
}

void MySmartCar::motorSetEnabled(bool enabled)
{
	myMotor.setEnabled(enabled);
}

void MySmartCar::servoSetEnabled(bool enabled)
{
	myServo.setEnabled(enabled);
}

void MySmartCar::reset(void)
{
	myMotor.reset();
	myServo.reset();
}

void MySmartCar::onButtonPress(const uint8_t id)
{
	switch (id)
	{
	case 0:
		m_smartCarInstance->myMotor.setEnabled(!m_smartCarInstance->myMotor.isEnabled());
		break;

	case 1:
		m_smartCarInstance->myServo.getMagSenRange(2000);
		m_smartCarInstance->myServo.setEnabled(!m_smartCarInstance->myServo.isEnabled());
		break;

	default:
		assert(false);
		// no break
	}
}
