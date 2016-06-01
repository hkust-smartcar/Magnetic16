/*
 *	pMotor.cpp
 *	
 *	Author: petel__
 *	Copyright (c) 2014-2016 HKUST SmartCar Team
 *	Refer to LICENSE for details
 *	
 */

#include "pMotor.h"

using namespace libsc;

DirMotor::Config getMotorConfig(const uint8_t id)
{
	DirMotor::Config config;
	config.id = id;
	return config;
}

pMotor::pMotor(const uint8_t id, MappingFunc mapingFunction, float &kP, float &kI, float &kD)
:
	DirMotor(getMotorConfig(id)),
	m_encoder(id),
	m_pid(pPid::PidParam(kP, kI, kD, m_setPoint, 500, -500))
{}

void pMotor::update(void)
{
	m_encoder.update();
	float tempPower = m_pid.getOutput(m_encoder.getSpeedCount());
	SetClockwise(tempPower > 0.0f);
	SetPower((uint16_t)ABS(tempPower));
}

float pMotor::getSpeedMs(void) const
{
	return m_encoder.getSpeedMs();
}

void pMotor::setSpeedMs(const int32_t speed)
{

}

float pMotor::getSpeedCount(void) const
{
	return m_encoder.getSpeedCount();
}

void pMotor::setSpeedCount(const int32_t speed)
{

}
