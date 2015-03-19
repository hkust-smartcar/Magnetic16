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
#include <libsc/k60/system.h>

#include "MySmartCar.h"
#include "MyMotor.h"
#include "MyServo.h"
#include "MyConfig.h"
#include "MyVar.h"

using namespace libsc::k60;
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
	m_turningMode(&config.MySmartCarTurningMode),
	myLeds({Led({0, false}), Led({1, false}), Led({2, false}), Led({3, false})}),
	myButtons({Button(getButtonConfig(0)), Button(getButtonConfig(1))}),
	myLcd(config, vars, myLoop),
	myMotor(config, vars, myLoop),
	myServo(config, vars, myLoop),
	myConfig(config)
{
	m_smartCarInstance = this;
	reset();
}

void MySmartCar::showValue(void)
{
	m_smartCarInstance->myVarMng.sendWatchData();
}

void MySmartCar::startMainLoop(void)
{
	myLoop.addFunctionToLoop(&showValue, 1, 5);
	myLoop.start();
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
		//m_smartCarInstance->myServo.getMagSenRange(2000);
		//m_smartCarInstance->myServo.setEnabled(!m_smartCarInstance->myServo.isEnabled());
		break;

	default:
		assert(false);
		// no break
	}
}

void MySmartCar::doBlink(Byte id)
{
	myLeds[id].Switch();
}
