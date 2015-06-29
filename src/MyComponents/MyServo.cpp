/*
 * MyServo.cpp
 *
 * Author: Peter
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include <MyPid.h>
#include <array>
#include "MyMagSen.h"
#include "MyServo.h"
#include "MyLoop.h"
#include "MySmartCar.h"
#include "MyResource.h"

using namespace std;

/*
 * MagSen Normal Reading:
 *
 * SD: 1.6f
 * FD: 0.3f
 * HD: 1.6f
 *
 */

MyServo *MyServo::m_instance = nullptr;

MyServo::MyServo(void)
:
	TrsD05({ 0 }),
	m_allow90DegreeTurning(true),
	m_isPidNonLinear(true),
	m_lastError(0),
	m_lastDegree(900),
	m_lastTurningDirection(LastServoLockDirection::None),
	m_MagSen({ MyMagSen(MyMagSen::MagSen::SD),
			   MyMagSen(MyMagSen::MagSen::FD),
			   MyMagSen(MyMagSen::MagSen::HD) }),
	m_servoPid({ MyPid(MyResource::ConfigTable::ServoConfig::Reference,
					   MyResource::ConfigTable::ServoConfig::NormalKp,
					   MyResource::ConfigTable::ServoConfig::NormalKi,
					   MyResource::ConfigTable::ServoConfig::NormalKd,
					   MyPid::ServoLinear,
					   -MAX_SERVO_ANGLE,
					   MAX_SERVO_ANGLE,
					   -1.0f,
					   1.0f),

				MyPid(MyResource::ConfigTable::ServoConfig::Reference,
					  MyResource::ConfigTable::ServoConfig::TurningKpA,
					  MyResource::ConfigTable::ServoConfig::TurningKi,
					  MyResource::ConfigTable::ServoConfig::TurningKd,
					  MyPid::ServoNonLinear,
					  -MAX_SERVO_ANGLE,
					  MAX_SERVO_ANGLE,
					  -1.0f,
					  1.0f,
					  MyResource::ConfigTable::ServoConfig::TurningKpA,
					  MyResource::ConfigTable::ServoConfig::TurningKpB)
				}),
	m_normal_weight({ &MyResource::ConfigTable::ServoConfig::NormalWeightSD, &MyResource::ConfigTable::ServoConfig::NormalWeightFD, &MyResource::ConfigTable::ServoConfig::NormalWeightHD }),
	m_turning_weight({ &MyResource::ConfigTable::ServoConfig::TurningWeightSD, &MyResource::ConfigTable::ServoConfig::TurningWeightFD, &MyResource::ConfigTable::ServoConfig::TurningWeightHD }),
	m_forceTurningFlag(false),
	m_enabled(true),
	m_pidChangedFlag(false)
{
	if (!m_instance)
		m_instance = this;
	reset();
}

void MyServo::reset(void)
{
	for (int i = 0; i < 2; i++)
		m_servoPid[i].reset();
	m_lastError = 0;
	for (uint8_t i = 0; i < m_MagSen.size(); i++)
		m_MagSen[i].reset();
	SetDegree(m_lastDegree);
}

float MyServo::getFinalAngle(void)
{
	m_lastError = 0.0f;

	if ((m_isPidNonLinear = (bool)(ABS(m_MagSen[MyMagSen::MagSen::SD].getOutputValue()) >= MyResource::ConfigTable::ServoConfig::TurningThresholdSdValue ||
			ABS(m_MagSen[MyMagSen::MagSen::HD].getOutputValue()) >= 0.2f)) ||
		(m_MagSen[MyMagSen::MagSen::SD].getFilteredValueAvg() <= 1.0f && m_MagSen[MyMagSen::MagSen::HD].getFilteredValueAvg() <= 1.5f))
		for (uint8_t i = 0; i < m_MagSen.size(); i++)
			m_lastError += m_MagSen[i].getValue() * *m_turning_weight[i];
	else
		for (uint8_t i = 0; i < m_MagSen.size(); i++)
			m_lastError += m_MagSen[i].getValue() * *m_normal_weight[i];

	if (m_90DegreeTurningNeed = check90Degree() && m_allow90DegreeTurning)
	{
//		MyResource::smartCar().m_buzzer.beep(1, 5);
		if (m_lastTurningDirection && m_lastTurningDirection == LastServoLockDirection::Right)
			m_lastError = 10.0f;
		else
			m_lastError = -10.0f;
	}

//		MyResource::smartCar().m_buzzer.beep(1, 10);

	return m_servoPid[(uint8_t)(m_isPidNonLinear)].update(m_lastError);
}

bool MyServo::check90Degree(void)
{
//	if (m_MagSen[MyMagSen::MagSen::SD].getFilteredValueAvg() < MyResource::ConfigTable::MotorConfig::EmergencyStopThreshold && m_MagSen[MyMagSen::MagSen::FD].getFilteredValueAvg() < MyResource::ConfigTable::MotorConfig::EmergencyStopThreshold && m_MagSen[MyMagSen::MagSen::HD].getFilteredValueAvg() < MyResource::ConfigTable::MotorConfig::EmergencyStopThreshold)
//		MyResource::smartCar().m_motor.setEnabled(false);

	// Maybe SD <= 1.5f
	if (m_MagSen[MyMagSen::MagSen::SD].getFilteredValueAvg() <= 1.0f && /*m_MagSen[MyMagSen::MagSen::FD].getFilteredValueAvg() <= 1.5f &&*/ m_MagSen[MyMagSen::MagSen::HD].getFilteredValueAvg() <= 1.5f)
	{
		if (!m_lastTurningDirection)
			m_lastTurningDirection = (LastServoLockDirection)((uint8_t)(m_MagSen[MyMagSen::MagSen::FD].getOutputValue() >= 0.0f) + 1);
//		MyResource::smartCar().m_buzzer.beep(1, 10);
		return true;
	}
	else
	{
		m_lastTurningDirection = LastServoLockDirection::None;
		return false;
	}
}

void MyServo::setAngle(const int16_t angle)
{
	if (m_enabled)
	{
		m_lastDegree = inRange(-MAX_SERVO_ANGLE, angle, MAX_SERVO_ANGLE) + MID_SERVO_ANGLE;
		SetDegree(m_lastDegree);
	}
}

void MyServo::setDegree(const uint16_t degree)
{
	if (m_enabled)
	{
		m_lastDegree = inRange(MIN_SERVO_DEGREE, degree, MAX_SERVO_DEGREE);
		SetDegree(m_lastDegree);
	}
}

void MyServo::updateAngle(const uint32_t &)
{
	m_instance->m_finalAngle = (int16_t)m_instance->getFinalAngle();

	if (m_instance->m_enabled)
		m_instance->setAngle(m_instance->m_finalAngle);
	else
		m_instance->setAngle(0);
}

void MyServo::setEnabled(const bool enabled)
{
	m_enabled = enabled;
}

bool MyServo::isEnabled(void)
{
	return m_enabled;
}
