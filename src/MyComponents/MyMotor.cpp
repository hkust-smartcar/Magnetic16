/*
 * MyMotor.cpp
 *
 * Author: Peter
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include <libsc/dir_motor.h>
#include <libsc/ab_encoder.h>
#include "MySmartCar.h"
#include "MyLcd.h"
#include "MyMotor.h"
#include "MyResource.h"
#include "MyLoop.h"

using namespace libsc;

MyMotor *MyMotor::m_instance = nullptr;

DirMotor::Config getMotorConfig(uint8_t id)
{
	DirMotor::Config config;
	config.id = id;
	return config;
}

MyMotor::MyMotor(void)
:
	DirMotor(getMotorConfig(0)),
	m_encoder(),
	m_speedPid(MyResource::ConfigTable::MotorConfig::Reference, MyResource::ConfigTable::MotorConfig::Kp, MyResource::ConfigTable::MotorConfig::Ki, MyResource::ConfigTable::MotorConfig::Kd, MyPid::Motor, -MAX_MOTOR_POWER, MAX_MOTOR_POWER, -1.0f, 1.0f),
	m_speed(0),
	m_enabled(false)
{
	if (!m_instance)
		m_instance = this;
	reset();
}

void MyMotor::setSpeed(int16_t speed)
{
	if (m_enabled)
	{
		if (speed > 0)
			SetClockwise(true);
		else
			SetClockwise(false);

		m_speed = inRange(-MAX_MOTOR_POWER, speed, MAX_MOTOR_POWER);
		SetPower(ABS(m_speed));
	}
	else
		SetPower(0);
}

void MyMotor::updateSpeed(const uint32_t &)
{
	m_instance->setSpeed(m_instance->m_speedPid.update(m_instance->m_encoder.getEncoderReading()));
}

int16_t *MyMotor::getSpeed(void)
{
	return &m_speed;
}

void MyMotor::setEnabled(const bool enabled)
{
	reset();
	m_encoder.reset();
	m_speedPid.reset();
	m_enabled = enabled;
}

bool MyMotor::isEnabled(void)
{
	return m_enabled;
}

void MyMotor::reset(void)
{
	m_speed = 0;
	SetPower(0);
	SetClockwise(true);
	m_speedPid.reset();
}
