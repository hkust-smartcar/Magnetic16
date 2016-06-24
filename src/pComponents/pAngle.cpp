/*
 *	pAngle.cpp
 *	
 *	Author: petel__
 *	Copyright (c) 2014-2016 HKUST SmartCar Team
 *	Refer to LICENSE for details
 *	
 */

#include "pAngle.h"

using namespace libsc;
using namespace std;

Mma8451q::Config getAccelConfig(void)
{
	Mma8451q::Config config;
	config.id = 0;
	config.power_mode = Mma8451q::Config::PowerMode::kHighResolution;
	return config;
}

Mpu6050::Config getGyroConfig(void)
{
	Mpu6050::Config config;
	config.accel_range = Mpu6050::Config::Range::kSmall;
	config.gyro_range = Mpu6050::Config::Range::kSmall;
	config.cal_drift = true;
	return config;
}

pAngle::pAngle(pAngle::Config config)
:
	Mpu6050(getGyroConfig()),
	m_param(config),
	m_lastAngle(0),
	m_lastYawOmega(0),
	m_accelAngle(0),
	m_gyroAngle(0),
	m_gyroOffset(0),
	m_lastTime(0)
{
	System::Init();
}

void pAngle::update(void)
{
	if (m_lastTime)
	{
		UpdateF();

		m_lastAccel = GetAccelF();
		m_lastOmega = GetOmegaF();

		float dt = (System::Time() - m_lastTime) / 1000.0f;

//		float tempAngleDiff = m_lastOmega[0] * dt;
//		m_gyroAngle += tempAngleDiff;
//		m_gyroOffset = *m_param.accelTrustValue * ((m_accelAngle = asin(inRange(-1.0f, -m_lastAccel[1], 1.0f)) * RadToDeg) - m_lastAngle);
//		m_lastAngle = m_gyroAngle + m_gyroOffset;
//		m_lastYawOmega = m_lastOmega[1] * sin(m_lastAngle * DegToRad);
		m_gyroOffset = ((m_accelAngle = asin(inRange(-1.0f, -m_lastAccel[1], 1.0f)) * RadToDeg) - m_lastAngle) / *m_param.accelTrustValue;
		m_gyroAngle += m_lastOmega[0] * dt;
		m_lastAngle += (m_lastOmega[0] + m_gyroOffset) * dt;
		m_lastYawOmega = m_lastOmega[1] * sin(m_lastAngle * DegToRad);
	}
	else
	{
		UpdateF();
		m_lastAccel = GetAccelF();
		m_lastAngle = m_gyroAngle = asin(inRange(-1.0f, -m_lastAccel[1], 1.0f)) * RadToDeg;
	}

	m_lastTime = System::Time();
}

float pAngle::getAngle(void) const
{
	return m_lastAngle;
}

float pAngle::getYawOmega(void) const
{
	return m_lastYawOmega;
}

array<float, 3> pAngle::getAccel(void) const
{
	return m_lastAccel;
}

array<float, 3> pAngle::getOmega(void) const
{
	return m_lastOmega;
}

float &pAngle::getAccel(const uint8_t index)
{
	return m_lastAccel[index];
}

float &pAngle::getOmega(const uint8_t index)
{
	return m_lastOmega[index];
}

float &pAngle::getAccelAngle(void)
{
	return m_accelAngle;
}

float &pAngle::getGyroAngle(void)
{
	return m_gyroAngle;
}
