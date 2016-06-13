/*
 *	pMotor.cpp
 *	
 *	Author: petel__
 *	Copyright (c) 2014-2016 HKUST SmartCar Team
 *	Refer to LICENSE for details
 *	
 */

#include <pMotor.h>
#include <pResource.h>

using namespace libsc;
using namespace std;

DirMotor::Config getMotorConfig(const uint8_t id)
{
	DirMotor::Config config;
	config.id = id;
	return config;
}

pMotor::pMotor(Config config)
:
	DirMotor(getMotorConfig(config.motorId)),
	m_config(config),
	m_encoder(config.encoderId, config.isEncoderrInverse),
	m_enabled(false),
	m_isInverse(config.isMotorInverse)
{}

void pMotor::update(void)
{
	m_encoder.update();
}

void pMotor::reset(void)
{
	setPower(0);
	m_encoder.reset();
}

void pMotor::setEnabled(const bool enabled)
{
	reset();
	m_enabled = enabled;
}

bool pMotor::isEnabled(void)
{
	return m_enabled;
}

void pMotor::setPower(const float power)
{
	if (m_enabled)
	{
		SetClockwise((power > 0) ^ m_isInverse);
		m_lastPower = (int16_t)power;
		SetPower(ABS(m_lastPower));
	}
}

void pMotor::setMappedPower(const float speed)
{
	if (m_enabled)
	{
		float tempPower = m_config.mappingFunction(speed);
		SetClockwise((tempPower > 0) ^ m_isInverse);
		m_lastPower = (int16_t)tempPower;
		SetPower(ABS(m_lastPower));
	}
}

int16_t &pMotor::getPower(void)
{
	return	m_lastPower;
}

int16_t &pMotor::getEncoderCount(void)
{
	return m_encoder.getEncoderCount();
}
