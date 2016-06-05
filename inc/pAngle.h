/*
 *	pAngle.h
 *	
 *	Author: petel__
 *	Copyright (c) 2014-2016 HKUST SmartCar Team
 *	Refer to LICENSE for details
 *	
 */

#pragma once

#include <array>
#include <cmath>
#include <libsc/mma8451q.h>
#include <libsc/mpu6050.h>
#include <libsc/timer.h>
#include <libsc/system.h>

using namespace libsc;
using namespace std;

#define RadToDeg 57.29577951f
#define DegToRad 0.017453293f
#define inRange(n, v, x) ((v > x)? x : ((v < n)? n : v))

class pAngle : private Mma8451q, private Mpu6050
{

public:

	struct Config
	{
		float accelTrustValue;

		float cgHeight;

		Config(float _accelTrustValue, float _cgHeight)
		:
			accelTrustValue(_accelTrustValue),
			cgHeight(_cgHeight)
		{}
	};

	enum Axis
	{
		x = 0,
		y,
		z
	};

	pAngle(Config config);

	void update(void);

	float getAngle(void) const;
	float getSpeed(void) const;
	float getYawOmega(void) const;

	array<float, 3> getAccel(void) const;
	array<float, 3> getOmega(void) const;
	float &getAccel(const uint8_t index);
	float &getOmega(const uint8_t index);

private:

	Config			m_param;
	float			m_lastSpeed;
	float			m_lastAngle;
	float			m_lastYawOmega;
	float			m_gyroAngle;
	float			m_gyroOffset;
	array<float, 3>	m_lastAccel;
	array<float, 3>	m_lastOmega;

	Timer::TimerInt	m_lastTime;

};
