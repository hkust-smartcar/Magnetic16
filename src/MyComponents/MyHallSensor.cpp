/*
 * MyHallSensor.cpp
 *
 * Author: Peter Lau
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include "MyHallSensor.h"
#include "MySmartCar.h"

using namespace libbase::k60;

MyHallSensor *MyHallSensor::m_instance = nullptr;

Gpi::Config MyHallSensor::getGpiConfig(void)
{
	Gpi::Config config;
	config.pin = LIBSC_I2C0_SCL;
	config.interrupt = Pin::Config::Interrupt::kFalling;
	config.isr = &onDetect;
	return config;
}

MyHallSensor::MyHallSensor(void)
:
	m_gpi(getGpiConfig()),
	m_isStarted(false),
	m_isTheEnd(false)
{
	if (!m_instance)
		m_instance = this;
}

void MyHallSensor::reset(void)
{
	m_isTheEnd = false;
	m_isStarted = false;
}

void MyHallSensor::onDetect(Gpi *target)
{
	if (target->Get())
		if (m_instance->m_isStarted)
		{
			m_instance->m_isTheEnd = true;
			MyResource::smartCar().m_buzzer.set(true);
		}
		else
			m_instance->m_isStarted = true;
}

bool MyHallSensor::isTheEnd(void)
{
	return m_isTheEnd;
}
