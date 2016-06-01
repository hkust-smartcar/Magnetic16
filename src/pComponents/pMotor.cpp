/*
 *	pMotor.cpp
 *	
 *	Author: petel__
 *	Copyright (c) 2014-2016 HKUST SmartCar Team
 *	Refer to LICENSE for details
 *	
 */

#include "pMotor.h"
#include <pResource.h>

using namespace libsc;

DirMotor::Config getMotorConfig(const uint8_t id)
{
	DirMotor::Config config;
	config.id = id;
	return config;
}

pMotor::pMotor(Config config)
:
	DirMotor(getMotorConfig(config.motorId)),
	m_encoder(config.encoderId, config.isEncoderrInverse),
	m_pid(pPid::PidParam(config.kP, config.kI, config.kD, m_setPoint, 500, -500)),
	m_isInverse(config.isMotorInverse),
	m_setPoint(0.0f)
{}

void pMotor::update(const float angle) // Temp criterion
{
	m_encoder.update();
	float tempPower = m_pid.getOutput(angle);
	setPower((int16_t)tempPower);
}

void pMotor::setPower(const int16_t power)
{
	SetClockwise((power > 0) ^ m_isInverse);
	SetPower(ABS(power));
}

int16_t pMotor::getPower(void)
{
	return	((IsClockwise() ^ m_isInverse)? GetPower() : -GetPower());
}

float pMotor::getSpeedMs(void) const
{
	return m_encoder.getSpeedMs();
}

float pMotor::getSpeedCount(void) const
{
	return m_encoder.getSpeedCount();
}

void pMotor::setSetPoint(const float newSetPoint)
{
	m_setPoint = newSetPoint;
}
