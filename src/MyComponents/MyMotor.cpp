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
	m_curReference(MyResource::ConfigTable::MotorConfig::Reference),
	m_speedPid(MyResource::ConfigTable::MotorConfig::Reference,
			   MyResource::ConfigTable::MotorConfig::Kp,
			   0.0f,
			   MyResource::ConfigTable::MotorConfig::Kd,
			   MyPid::Motor,
			   -MAX_MOTOR_POWER,
			   MAX_MOTOR_POWER,
			   0.0f,
			   MyResource::ConfigTable::MotorConfig::Reference * 2),
	m_startTime(0),
	m_timePassed(0),
	m_power(0),
	m_speed(0.0f),
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

		m_power = inRange(-MAX_MOTOR_POWER, speed, MAX_MOTOR_POWER);
		SetPower(ABS(m_power));
	}
	else
		SetPower(0);
}

void MyMotor::updateSpeed(const uint32_t &)
{
//	if (MyResource::smartCar().m_servo.m_isPidNonLinear)
//		m_instance->m_timePassed = 0;
//	else
//		m_instance->m_timePassed += MyResource::ConfigTable::MotorConfig::UpdateFreq;

//	m_instance->m_curReference = MyResource::ConfigTable::MotorConfig::Reference * ((!MyResource::smartCar().m_servo.m_isPidNonLinear)? (1 + (float)inRange(0, ABS(m_instance->m_timePassed), MyResource::ConfigTable::MotorConfig::TimeForReachingMaxSpeed) / MyResource::ConfigTable::MotorConfig::TimeForReachingMaxSpeed * MyResource::ConfigTable::MotorConfig::MaxSpeedRatio) : ((MyResource::smartCar().m_servo.m_90DegreeTurningNeed)? MyResource::ConfigTable::MotorConfig::Turning90DegreeSpeedRatio : MyResource::ConfigTable::MotorConfig::TurningSpeedRatio));
//	m_instance->m_curReference = MyResource::ConfigTable::MotorConfig::Reference * ((m_isNonLinearPidNeed)? MyResource::ConfigTable::MotorConfig::TurningSpeedRatio : 1.0f);

	float output = m_instance->m_speedPid.update(m_instance->m_encoder.getEncoderReading());

	if (MyResource::smartCar().m_hallSensor.isTheEnd()/* || (output == MAX_MOTOR_POWER && System::Time() - m_instance->m_startTime > 2000)*/)
	{
		m_instance->setEnabled(false);
		m_instance->setSpeed(0);
	}

	if (System::Time() - MyResource::smartCar().m_lastTimeUpdateKey >= 200)
		output = 0;

	m_instance->setSpeed(output);
//	m_instance->setSpeed(200);
}

int16_t *MyMotor::getPower(void)
{
	return &m_power;
}

float &MyMotor::getSpeed(void)
{
	return m_speed;
}

void MyMotor::setEnabled(const bool enabled)
{
	if (!MyResource::smartCar().m_lcdConsole.isEnabled() || !MyResource::smartCar().m_mode)
	{
		reset();
		m_encoder.reset();
		m_speedPid.reset();
		MyResource::smartCar().m_hallSensor.reset();
//		if (m_enabled = enabled)
//			m_startTime = System::Time() - MyResource::ConfigTable::MotorConfig::UpdateFreq;
		m_enabled = enabled;
	}
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
