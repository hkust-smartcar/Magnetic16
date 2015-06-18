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

MyServo *MyServo::m_instance = nullptr;

MyServo::MyServo(void)
:
	TrsD05({ 0 }),
	m_isPidNonLinear(true),
	m_lastError(0),
	m_lastDegree(900),
	m_lastTurningDirection(Direction::LEFT),
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
					  MyResource::ConfigTable::ServoConfig::NormalKp,
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
	m_weight({ &MyResource::ConfigTable::ServoConfig::WeightSD, &MyResource::ConfigTable::ServoConfig::WeightFD, &MyResource::ConfigTable::ServoConfig::WeightHD }),
	m_forceTurningFlag(false),
	m_enabled(false),
	m_pidChangedFlag(false),
	m_dataCollector_90({ 0.0f, 0.0f, 0.0f, Direction::LEFT, false, false, false, false, false, true })
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
	for (uint8_t i = 0; i < m_MagSen.size(); i++)
		m_lastError += m_MagSen[i].getValue() * *m_weight[i];

	if (m_90DegreeTurningNeed = (uint8_t)check90Degree())
	{
		MyResource::smartCar().m_buzzer.beep(1, 5);
		if ((bool)m_dataCollector_90.turningDirection)
			m_lastError = 10.0f;
		else
			m_lastError = -10.0f;
	}

//		MyResource::smartCar().m_buzzer.beep(1, 10);

	m_isPidNonLinear = (ABS(m_MagSen[MyMagSen::MagSen::SD].getOutputValue()) >= MyResource::ConfigTable::ServoConfig::TurningThresholdSdValue);

	return m_servoPid[(uint8_t)(m_isPidNonLinear)].update(m_lastError);
}

bool MyServo::check90Degree(void)
{
//	if (!m_dataCollector_90.isFdReady && ABS(m_MagSen[MyMagSen::MagSen::FD].getOutputValue()) > MyResource::ConfigTable::ServoConfig::ForceTurningThresholdStartFdOutput && m_dataCollector_90.isReset)
//	{
//		m_dataCollector_90.isFdReady = true;
		m_dataCollector_90.turningDirection = (Direction)(m_MagSen[MyMagSen::MagSen::FD].getOutputValue() >= 0.0f);
//		m_dataCollector_90.isReset = false;
//	}
//	else if (!m_dataCollector_90.isSdReady && ABS(m_MagSen[MyMagSen::MagSen::SD].getOutputValue()) > MyResource::ConfigTable::ServoConfig::ForceTurningThresholdStartSdOutput && !m_dataCollector_90.isReset)
//		m_dataCollector_90.isSdReady = true;
//	else if (!m_dataCollector_90.isReset && m_dataCollector_90.isFdReachedLocMin && m_dataCollector_90.isSdReachedLocMax && (ABS(m_MagSen[MyMagSen::MagSen::FD].getOutputValue()) < MyResource::ConfigTable::ServoConfig::ForceTurningThresholdStopFdOutput || ((m_dataCollector_90.prevHd <= 0.0f && m_MagSen[MyMagSen::MagSen::HD].getOutputValue() >= 0.0f) || (m_dataCollector_90.prevHd >= 0.0f && m_MagSen[MyMagSen::MagSen::HD].getOutputValue() <= 0.0f))))
//	{
//		m_dataCollector_90.prevSd = 0.0f;
//		m_dataCollector_90.prevFd = 0.0f;
//		m_dataCollector_90.prevHd = 0.0f;
//		m_dataCollector_90.isFdReady = false;
//		m_dataCollector_90.isFdReachedLocMax = false;
//		m_dataCollector_90.isFdReachedLocMin = false;
//		m_dataCollector_90.isSdReady = false;
//		m_dataCollector_90.isSdReachedLocMax = false;
//		m_dataCollector_90.isReset = true;
//		return false;
//	}
//
//	if (m_dataCollector_90.isFdReady && !m_dataCollector_90.isFdReachedLocMax && ABS(m_MagSen[MyMagSen::MagSen::FD].getOutputValue()) < m_dataCollector_90.prevFd)
//		m_dataCollector_90.isFdReachedLocMax = true;
//	else if (m_dataCollector_90.isFdReady && m_dataCollector_90.isFdReachedLocMax && !m_dataCollector_90.isFdReachedLocMin && ABS(m_MagSen[MyMagSen::MagSen::FD].getOutputValue()) > m_dataCollector_90.prevFd)
//		m_dataCollector_90.isFdReachedLocMin = true;
//
//	if (m_dataCollector_90.isSdReady && m_dataCollector_90.isFdReady && m_dataCollector_90.isFdReachedLocMax && !m_dataCollector_90.isSdReachedLocMax && ABS(m_MagSen[MyMagSen::MagSen::SD].getOutputValue()) < m_dataCollector_90.prevSd)
//		m_dataCollector_90.isSdReachedLocMax = true;
//
//	m_dataCollector_90.prevSd = m_MagSen[MyMagSen::MagSen::SD].getOutputValue();
//	m_dataCollector_90.prevFd = m_MagSen[MyMagSen::MagSen::FD].getOutputValue();
//	m_dataCollector_90.prevHd = m_MagSen[MyMagSen::MagSen::HD].getOutputValue();

//	if (ABS(m_MagSen[MyMagSen::MagSen::FD].getFilteredValueAvg()) > 1.9f)
//		MyResource::smartCar().m_buzzer.beep(1, 7);

//	return (m_dataCollector_90.isFdReachedLocMin && m_dataCollector_90.isSdReachedLocMax);
//	return (ABS(m_MagSen[MyMagSen::MagSen::FD].getFilteredValueAvg()) > 1.85f);
	return (m_MagSen[MyMagSen::MagSen::SD].getFilteredValueAvg() <= 1.0f && m_MagSen[MyMagSen::MagSen::SD].getFilteredValueAvg() <= 1.5f);
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
	int16_t finalAngle = (int16_t)m_instance->getFinalAngle();

	if (m_instance->m_enabled)
		m_instance->setAngle(finalAngle);
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
