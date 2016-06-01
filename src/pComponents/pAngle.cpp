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
	config.accel_range = Mpu6050::Config::Range::kLarge;
	config.gyro_range = Mpu6050::Config::Range::kSmall;
	config.cal_drift = true;
	return config;
}

pAngle::pAngle(pAngle::Config config)
:
	Mma8451q(getAccelConfig()),
	Mpu6050(getGyroConfig()),
	m_param(config),
	m_lastAngle(0),
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
		this->Mma8451q::UpdateF();
		this->Mpu6050::UpdateF();

		m_lastAccel = this->Mma8451q::GetAccelF();
		m_lastOmega = GetOmegaF();

		m_gyroAngle -= m_lastOmega[0] * (System::Time() - m_lastTime) / 1000;
		m_gyroOffset += m_param.accelTrustValue * (asin(inRange(-1.0f, m_lastAccel[0], 1.0f)) * RadToDeg - m_lastAngle);
		m_lastAngle = m_gyroAngle + m_gyroOffset;
	}
	else
	{
		this->Mma8451q::UpdateF();
		m_lastAccel = this->Mma8451q::GetAccelF();
		m_lastAngle = m_gyroOffset = asin(inRange(-1.0f, m_lastAccel[0], 1.0f)) * RadToDeg;
	}

	m_lastTime = System::Time();
}

float pAngle::getAngle(void) const
{
	return m_lastAngle;
}

array<float, 3> pAngle::getAccel(void) const
{
	return m_lastAccel;
}

array<float, 3> pAngle::getOmega(void) const
{
	return m_lastOmega;
}

float pAngle::getAccel(const uint8_t index) const
{
	return m_lastAccel[index];
}

float pAngle::getOmega(const uint8_t index) const
{
	return m_lastOmega[index];
}
