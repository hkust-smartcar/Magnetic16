/*
 * MyServo.h
 *
 *  Created on: Apr 25, 2015
 *      Author: Peter
 */

#include <array>
#include "MyMagSen.h"
#include "MyServo.h"
#include "MyLoop.h"
#include "MyPID.h"
#include "MyResource.h"

using namespace std;

MyServo *MyServo::m_instance = nullptr;

MyServo::MyServo(void)
:
	TrsD05({ 0 }),
	m_MagSen({ MyMagSen(MyMagSen::MagSen::SD),
			   MyMagSen(MyMagSen::MagSen::FD),
			   MyMagSen(MyMagSen::MagSen::HD) }),
	m_servoPID(MyResource::ConfigTable::ServoConfig::Reference,
			   MyResource::ConfigTable::ServoConfig::Kp,
			   MyResource::ConfigTable::ServoConfig::Ki,
			   MyResource::ConfigTable::ServoConfig::Kd,
			   MyPID::Servo,
			   MIN_SERVO_ANGLE,
			   MAX_SERVO_ANGLE,
			   -1.0f,
			   1.0f),
	m_lastAngle(0),
	m_weight({ &MyResource::ConfigTable::ServoConfig::WeightSD, &MyResource::ConfigTable::ServoConfig::WeightFD, &MyResource::ConfigTable::ServoConfig::WeightHD }),
	m_enabled(false)
{
	if (!m_instance)
		m_instance = this;
	reset();
}

void MyServo::reset(void)
{
	m_servoPID.reset();
	m_lastAngle = 0;
	for (uint8_t i = 0; i < m_MagSen.size(); i++)
		m_MagSen[i].reset();
}

float MyServo::getFinalAngle(void)
{
	m_lastAngle = 0.0f;
	for (uint8_t i = 0; i < m_MagSen.size(); i++)
		m_lastAngle += m_MagSen[i].getValue() * *m_weight[i];


	if (m_MagSen[MyMagSen::MagSen::SD].getFilteredValueAvg() < 0.2f && m_MagSen[MyMagSen::MagSen::FD].getFilteredValueAvg() < 0.2f)
		if ((bool)m_lastTurningDirection)
			m_lastAngle = 2.0f;
		else
			m_lastAngle = -2.0f;
	else
		m_lastTurningDirection = (Direction)(m_MagSen[MyMagSen::MagSen::FD].getOutputValue() >= 0.0f);

	return m_servoPID.update(m_lastAngle);
}

void MyServo::setDegree(const int16_t degree)
{
	if (m_enabled)
	{
		m_lastDegree = degree;
		SetDegree(m_lastDegree);
	}
}

void MyServo::updateAngle(void)
{
	int16_t finalAngle = (int16_t)getFinalAngle();

	if (m_enabled)
		setDegree(MID_SERVO_ANGLE - finalAngle);
	else
		setDegree(MID_SERVO_ANGLE);
}

void MyServo::setEnabled(const bool enabled)
{
	m_enabled = enabled;
}

bool MyServo::isEnabled(void)
{
	return m_enabled;
}
