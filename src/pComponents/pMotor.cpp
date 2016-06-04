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
using namespace std;

DirMotor::Config getMotorConfig(const uint8_t id)
{
	DirMotor::Config config;
	config.id = id;
	return config;
}

pPid::PidParam getPidConfig(const pMotor::Config &config)
{
	pPid::PidParam param;
	param.kPFunc = function<float (float)>([config](float error) { if (ABS(error) < 0.3f) return 0.0f; else return config.kP * sin(error * DegToRad); });
	param.kI = &config.kI;
	param.kD = &config.kD;
	param.setPoint = &pResource::configTable.kIdealAngle;
	param.max = 500;
	param.min = -500;
	return param;
}

pMotor::pMotor(Config config)
:
	DirMotor(getMotorConfig(config.motorId)),
	m_config(config),
	m_kPFunc([&](float val) { return config.kP * sin(val * DegToRad); }),
	m_encoder(config.encoderId, config.isEncoderrInverse),
	m_pid(getPidConfig(config)),
	m_isInverse(config.isMotorInverse),
	m_setPoint(0.0f)
{}

void pMotor::update(const float angle) // Temp criterion
{
	m_encoder.update();
	setSpeed((int16_t)m_pid.getOutput(angle));
}
void pMotor::update(void)
{
	m_encoder.update();
}

void pMotor::setPower(const int16_t power)
{
	SetClockwise((power > 0) ^ m_isInverse);
	SetPower(ABS(power));
	m_lastPower = power;
}

void pMotor::setSpeed(const int16_t speed)
{
	int16_t tempPower = (int16_t)m_config.mappingFunction(speed);
	SetClockwise((tempPower > 0) ^ m_isInverse);
	SetPower(ABS(tempPower));
	m_lastPower = tempPower;
}

int16_t &pMotor::getPower(void)
{
	return	m_lastPower;
}

int32_t &pMotor::getSpeedCount(void)
{
	return m_encoder.getSpeedCount();
}

void pMotor::setSetPoint(const float newSetPoint)
{
	m_setPoint = newSetPoint;
}

void pMotor::setKp(const float kP)
{
	m_config.kP = kP;
}
float pMotor::getKp(void)
{
	return m_config.kP;
}

void pMotor::setKi(const float kI)
{
	m_config.kI = kI;
}
float pMotor::getKi(void)
{
	return m_config.kI;
}

void pMotor::setKd(const float kD)
{
	m_config.kD = kD;
}
float pMotor::getKd(void)
{
	return m_config.kD;
}
