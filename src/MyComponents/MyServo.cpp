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
//	m_normal_weight({ &MyResource::ConfigTable::ServoConfig::NormalWeightSD, &MyResource::ConfigTable::ServoConfig::NormalWeightFD, &MyResource::ConfigTable::ServoConfig::NormalWeightHD }),
//	m_turning_weight({ &MyResource::ConfigTable::ServoConfig::TurningWeightSD, &MyResource::ConfigTable::ServoConfig::TurningWeightFD, &MyResource::ConfigTable::ServoConfig::TurningWeightHD }),
	m_90DegreeChecker({ 0, false, LastServoLockDirection::None }),
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

	DelayMsByTicks(5000);

	float voltageSum[6] = { 0.0f };
	for (uint8_t i = 0; i < 20; i++)
	{
		updateMagSen();
		for (uint8_t j = 0; j < 3; j++)
		{
			array<float, 2> tempVoltage = m_MagSen[j].getRawValue();
			for (uint8_t k = 0; k < 2; k++)
				voltageSum[j * 2 + k] += tempVoltage[k];
		}
		DelayMsByTicks(10);
	}

	for (uint8_t i = 0; i < 3; i++)
		m_MagSen[i].init(new float[2] { 20 / voltageSum[i * 2], 20 / voltageSum[i * 2 + 1] });
}

inline float getRealReading(bool triggerCondition, float input, float valueIfTrigger)
{
	if (triggerCondition)
		if (input >= 0.0f)
			return valueIfTrigger;
		else
			return -valueIfTrigger;
	else
		return input;
}

void MyServo::updateMagSen(void)
{
	for (uint8_t i = 0; i < m_MagSen.size(); i++)
		m_MagSen[i].update();

	float tempFdOutput = m_MagSen[MyMagSen::MagSen::FD].getOutputValue();

	if (ABS(tempFdOutput) > MyResource::ConfigTable::ServoConfig::TurningThresholdFdValue)

}

float MyServo::getFinalAngle(void)
{
	m_lastError = 0.0f;
	updateMagSen();

//	if (m_isPidNonLinear = checkIsTurningNeeded())
//		for (uint8_t i = 0; i < m_MagSen.size(); i++)
//			m_lastError += m_MagSen[i].getValue() * *m_turning_weight[i];
//	else
//		for (uint8_t i = 0; i < m_MagSen.size(); i++)
//			m_lastError += m_MagSen[i].getValue() * *m_normal_weight[i];

	m_isPidNonLinear = turningHandler(m_lastError);
	m_90DegreeTurningNeed = check90Degree(m_lastError);

	m_servoPid[(uint8_t)(!m_isPidNonLinear)].update(m_lastError);
	return m_servoPid[(uint8_t)(m_isPidNonLinear)].update(m_lastError);
}

bool MyServo::turningHandler(float &error)
{
	float tempSdOutput = m_MagSen[MyMagSen::MagSen::SD].getOutputValue();
	float tempHdOutput = m_MagSen[MyMagSen::MagSen::HD].getOutputValue();

	error = tempSdOutput + tempHdOutput;

	return (ABS(tempHdOutput) >= MyResource::ConfigTable::ServoConfig::TurningThresholdHdValue ||
		ABS(m_MagSen[MyMagSen::MagSen::FD].getOutputValue()) > MyResource::ConfigTable::ServoConfig::TurningThresholdFdValue);

//	return (ABS(m_MagSen[MyMagSen::MagSen::FD].getOutputValue()) > MyResource::ConfigTable::ServoConfig::TurningThresholdFdValue);
}

bool MyServo::check90Degree(float &error)
{
	if (m_MagSen[MyMagSen::MagSen::SD].getFilteredValueAvg() < MyResource::ConfigTable::ServoConfig::NoSignalThreshold)
	{
		if (System::Time() - m_90DegreeChecker.lastTime <= 500 || m_90DegreeChecker.is90DegreeTurningStarted) // 0.5second
		{
			if (m_90DegreeChecker.hasReachThreshold)
			{
				if (m_90DegreeChecker.direction == LastServoLockDirection::Right)
					error = 10.0f;
				else if (m_90DegreeChecker.direction == LastServoLockDirection::Left)
					error = -10.0f;
				m_90DegreeChecker.is90DegreeTurningStarted = true;
				return true;
			}
		}
		else
		{
			m_90DegreeChecker.hasReachThreshold = false;
			m_90DegreeChecker.is90DegreeTurningStarted = false;
			m_90DegreeChecker.direction == LastServoLockDirection::None;
		}
	}
	else if (m_MagSen[MyMagSen::MagSen::FD].getFilteredValueAvg() < MyResource::ConfigTable::ServoConfig::Turning90DegreeThresholdFdAvg && m_90DegreeChecker.lastTime > 500)
	{
		m_90DegreeChecker.hasReachThreshold = true;
		m_90DegreeChecker.direction = (LastServoLockDirection)((uint8_t)(m_MagSen[MyMagSen::MagSen::FD].getOutputValue() >= 0.0f) + 1);
		m_90DegreeChecker.is90DegreeTurningStarted = false;
		m_90DegreeChecker.lastTime = System::Time();
	}
	else
	{
		m_90DegreeChecker.hasReachThreshold = false;
		m_90DegreeChecker.is90DegreeTurningStarted = false;
		m_90DegreeChecker.direction == LastServoLockDirection::None;
	}
	return false;
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
