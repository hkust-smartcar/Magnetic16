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

pMotor::pMotor(const uint8_t id)
:
	DirMotor(getMotorConfig(id)),
	m_encoder(id)
{}

uint32_t pMotor::getSpeedMs(void)
{
	return m_encoder.getSpeedMs();
}

void pMotor::setSpeedMs(const int32_t speed)
{

}

int32_t pMotor::getSpeedCount(void) const
{
	return m_encoder.getSpeedCount();
}

void pMotor::setSpeedCount(const int32_t speed)
{

}
