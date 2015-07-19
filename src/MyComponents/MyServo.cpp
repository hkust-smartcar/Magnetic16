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

inline float diff(float &oldN, float newN)
{
	float ret = newN - oldN;
	oldN = newN;
	return ret;
}

inline float diff(Timer::TimerInt &oldN, Timer::TimerInt newN)
{
	float ret = newN - oldN;
	oldN = newN;
	return ret;
}

MyServo::MyServo(void)
:
	TrsD05({ 0 }),
	m_allow90DegreeTurning(true),
	m_isPidNonLinear(true),
	m_lastError(0),
	m_lastDegree(900),
	m_lastTurningDirection(LastServoLockDirection::None),
	m_MagSen({ MyMagSen(MyMagSen::MagSenId::SD),
			   MyMagSen(MyMagSen::MagSenId::FD),
			   MyMagSen(MyMagSen::MagSenId::HD) }),
	m_servoPid({ MyPid(*(new float(0.0f)),
					   MyResource::ConfigTable::ServoConfig::NormalKp,
					   0.0f,
					   MyResource::ConfigTable::ServoConfig::NormalKd,
					   MyPid::ServoLinear,
					   -MAX_SERVO_ANGLE,
					   MAX_SERVO_ANGLE,
					   -2.0f,
					   2.0f),

				MyPid(*(new float(0.0f)),
					  MyResource::ConfigTable::ServoConfig::TurningKpA,
					  0.0f,
					  MyResource::ConfigTable::ServoConfig::TurningKd,
					  MyPid::ServoNonLinear,
					  -MAX_SERVO_ANGLE,
					  MAX_SERVO_ANGLE,
					  -2.0f,
					  2.0f,
					  MyResource::ConfigTable::ServoConfig::TurningKpA,
					  MyResource::ConfigTable::ServoConfig::TurningKpB,
					  MyResource::ConfigTable::ServoConfig::NormalKp)
				}),
//	m_normal_weight({ &MyResource::ConfigTable::ServoConfig::NormalWeightSD, &MyResource::ConfigTable::ServoConfig::NormalWeightFD, &MyResource::ConfigTable::ServoConfig::NormalWeightHD }),
//	m_turning_weight({ &MyResource::ConfigTable::ServoConfig::TurningWeightSD, &MyResource::ConfigTable::ServoConfig::TurningWeightFD, &MyResource::ConfigTable::ServoConfig::TurningWeightHD }),
	m_90DegreeChecker({ 0, false, false, LastServoLockDirection::Left }),
	m_enabled(true),
	m_pidChangedFlag(false),
	m_isTurning90Degree(false)
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

	float voltageSum[6] = { 0.0f };
	for (uint8_t i = 0; i < 40; i++)
	{
		updateMagSen();
		for (uint8_t j = 0; j < 3; j++)
		{
			array<float, 2> tempVoltage = m_MagSen[j].getRawValue();
			for (uint8_t k = 0; k < 2; k++)
				voltageSum[j * 2 + k] += tempVoltage[k];
		}
		DelayMsByTicks(7);
	}

	for (uint8_t i = 0; i < 3; i++)
		m_MagSen[i].init(new float[2] { 40 / voltageSum[i * 2], 40 / voltageSum[i * 2 + 1] });
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
}

float MyServo::getFinalAngle(void)
{
	m_lastError = 0.0f;
	updateMagSen();

//	m_dt = diff(m_lastCheckTime, System::Time());
//	m_deiffChangeL = diff(m_lastFdDiffL, m_MagSen[MyMagSen::MagSenId::FD].getFilteredValue()[0]) / m_dt;
//	m_deiffChangeR = diff(m_lastFdDiffR, m_MagSen[MyMagSen::MagSenId::FD].getFilteredValue()[1]) / m_dt;

	m_isPidNonLinear = turningHandler(m_lastError);
//	MyResource::smartCar().m_buzzer.set(m_isPidNonLinear);
	m_90DegreeTurningNeed = check90Degree(m_lastError);
//	checkSmallS(m_lastError);

	m_servoPid[(uint8_t)(!m_isPidNonLinear)].update(m_lastError);
	return m_servoPid[(uint8_t)(m_isPidNonLinear)].update(m_lastError);
}

bool MyServo::turningHandler(float &error)
{
	error = m_MagSen[MyMagSen::MagSenId::SD].getOutputValue() + m_MagSen[MyMagSen::MagSenId::HD].getOutputValue();

	return (m_MagSen[MyMagSen::MagSenId::SD].getFilteredValueAvg() < MyResource::ConfigTable::ServoConfig::SdNoSignalThreshold);
}

bool MyServo::check90Degree(float &error)
{
//	if (m_MagSen[MyMagSen::MagSenId::FD].getFilteredValueAvg() > MyResource::ConfigTable::ServoConfig::CrossRoadThresholdFdValue)
//	{
//		m_isTurning90Degree = false;
//		error = m_MagSen[MyMagSen::MagSenId::SD].getOutputValue();
//	}
	if (m_isTurning90Degree && ABS(m_MagSen[MyMagSen::MagSenId::SD].getOutputValue()) >= 0.6f)
	{
		m_isTurning90Degree = false;
		MyResource::smartCar().m_buzzer.set(false);
	}
//	else if (m_MagSen[MyMagSen::MagSenId::FD].getFilteredValue()[0] > 1.45f && m_MagSen[MyMagSen::MagSenId::FD].getFilteredValue()[1] < 1.0f ||
//			  m_MagSen[MyMagSen::MagSenId::FD].getFilteredValue()[1] > 1.45f && m_MagSen[MyMagSen::MagSenId::FD].getFilteredValue()[0] < 1.0f)
//	{
//		m_isTurning90Degree = true;
//		m_lastTurningDirection = (LastServoLockDirection)(m_MagSen[MyMagSen::MagSenId::FD].getFilteredValue()[1] > m_MagSen[MyMagSen::MagSenId::FD].getFilteredValue()[0]);
//		return true;
//	}
//	if (ABS(m_MagSen[MyMagSen::MagSenId::FD].getFilteredValueAvg()[0]) >= MyResource::ConfigTable::ServoConfig::Turning90DegreeThresholdFd || ABS(m_MagSen[MyMagSen::MagSenId::FD].getFilteredValueAvg()) >= MyResource::ConfigTable::ServoConfig::Turning90DegreeThresholdFd)
//	{
//		MyResource::smartCar().m_buzzer.set(true);
//		m_isTurning90Degree = true;
//		error = (m_MagSen[MyMagSen::MagSenId::FD].getOutputValue() > 0.0f)? 5.0f : -5.0f;
//	}
	else if (!m_isTurning90Degree && m_MagSen[MyMagSen::MagSenId::SD].getFilteredValueAvg() <= 0.7f && ABS(m_MagSen[MyMagSen::MagSenId::SD].getOutputValue()) <= 0.2f && ABS(m_MagSen[MyMagSen::MagSenId::FD].getOutputValue()) >= 0.4f/* && ABS(m_MagSen[MyMagSen::MagSenId::FD].getOutputValue()) > m_MagSen[MyMagSen::MagSenId::SD].getFilteredValueAvg()*/)
	{
		m_isTurning90Degree = true;
		MyResource::smartCar().m_buzzer.set(true);
		m_last90TurningDirection = (m_MagSen[MyMagSen::MagSenId::FD].getOutputValue() > 0.0f)? LastServoLockDirection::Left : LastServoLockDirection::Right;
	}

	if (m_isTurning90Degree)
		error = (m_last90TurningDirection - 1)? -5.0f : 5.0f;

	return m_last90TurningDirection;
}

bool MyServo::checkSmallS(float &error)
{
	if (isAround(m_MagSen[MyMagSen::MagSenId::SD].getFilteredValueAvg(), 1.0f, 0.3f) && m_MagSen[MyMagSen::MagSenId::SD].getOutputValue() > 0.3f)
		error = m_MagSen[MyMagSen::MagSenId::FD].getOutputValue();

	return false;
}

void MyServo::setAngle(const int16_t angle)
{
	if (m_enabled)
	{
		m_lastDegree = inRange(-MAX_SERVO_ANGLE, angle, MAX_SERVO_ANGLE) + MID_SERVO_ANGLE;
		m_lastTurningDirection = (LastServoLockDirection)((m_lastDegree >= MID_SERVO_ANGLE) + 1);
		SetDegree(m_lastDegree);
	}
}

void MyServo::setDegree(const uint16_t degree)
{
	if (m_enabled)
	{
		m_lastDegree = inRange(MIN_SERVO_DEGREE, degree, MAX_SERVO_DEGREE);
		m_lastTurningDirection = (LastServoLockDirection)((m_lastDegree >= MID_SERVO_ANGLE) + 1);
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
