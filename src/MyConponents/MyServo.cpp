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
	m_weight({ &MyResource::ConfigTable::ServoConfig::WeightSD, &MyResource::ConfigTable::ServoConfig::WeightFD, &MyResource::ConfigTable::ServoConfig::WeightHD })
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
	return m_servoPID.update(m_lastAngle);
}

void MyServo::setDegree(const int16_t degree)
{
	m_lastDegree = degree;
	SetDegree(m_lastDegree);
}

float MyServo::updateAngle(void)
{
	setDegree(MID_SERVO_ANGLE - (int16_t)getFinalAngle());
}
