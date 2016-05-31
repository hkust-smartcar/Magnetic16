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

bool pAngle::isReady(void) const
{
	int32_t dt = System::Time() - m_lastTime - m_param.updatePeriod;

	if (m_param.useStrict)
		assert(dt <= 0);

	return (dt == 0); // TODO: check if this works
}

void pAngle::update(void)
{
	if (m_lastTime)
	{
//		Mma8451q::Update();
//		Mpu6050::Update();
//
//		array<int16_t, 3> tempAccel = Mma8451q::GetAccel();
//		array<int32_t, 3> tempOmega = Mpu6050::GetOmega();
		this->Mma8451q::UpdateF();
		this->Mpu6050::UpdateF();

//		float accelAngle = asin(this->Mma8451q::GetAccelF()[0]) * RadToDeg;
		//float gyroAngle = m_lastAngle / (float)100 + GetOmegaF()[0] * (float)(System::Time() - m_lastTime) / 1000;

		m_gyroAngle += GetOmegaF()[0] * (System::Time() - m_lastTime) / 1000;
		m_gyroOffset += m_param.accelTrustValue * (asin(inRange(-1.0f, this->Mma8451q::GetAccelF()[0], 1.0f)) * RadToDeg - m_lastAngle);
		m_lastAngle = m_gyroAngle + m_gyroOffset;
	}
	else
	{
		this->Mma8451q::UpdateF();
		m_lastAngle = m_gyroOffset = asin(inRange(-1.0f, this->Mma8451q::GetAccelF()[0], 1.0f)) * RadToDeg;
	}

	m_lastTime = System::Time();
}

float pAngle::getAngle(void) const
{
	return m_lastAngle;
}
