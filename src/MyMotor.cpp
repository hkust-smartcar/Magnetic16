/*
 * MyMotor.cpp
 *
 * Author: Peter
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include <libsc/dir_motor.h>
#include <libsc/ab_encoder.h>
#include "MyMotor.h"
#include "MyResource.h"

using namespace libsc;

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
	m_speed(0),
	m_enabled(false),
	m_speedPID(&MyResource::ConfigTable::MotorConfig::Reference, &MyResource::ConfigTable::MotorConfig::Kp, &MyResource::ConfigTable::MotorConfig::Ki, &MyResource::ConfigTable::MotorConfig::Kd, -MAX_MOTOR_POWER, MAX_MOTOR_POWER)
{
	if (!m_instance)
		m_instance = new MyMotor;
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

void MyMotor::updateSpeed(void)
{
	//SetPower(m_speedPID.updatePID(m_encoder.GetCount()));
}

void MyMotor::setEnabled(const bool enabled)
{
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
	m_speedPID.reset();
}

void MyMotor::speedControlRoutine(void)
{
	m_instance->updateSpeed();
}
