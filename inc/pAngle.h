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

class pAngle : private Mpu6050, private Mma8451q
{

public:

	struct Config
	{
		float *accelTrustValue;

		Config(float &_accelTrustValue)
		:
			accelTrustValue(&_accelTrustValue)
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
	float getYawOmega(void) const;

	array<float, 3> getAccel(void) const;
	array<float, 3> getOmega(void) const;

	float &getAccel(const uint8_t index);
	float &getOmega(const uint8_t index);

	float &getAccelAngle(void);
	float &getGyroAngle(void);

private:

	Config			m_param;

	float			m_lastAngle;
	float			m_lastYawOmega;

	float			m_accelAngle;
	float			m_gyroAngle;

	float			m_gyroOffset;

	array<float, 3>	m_lastAccel;
	array<float, 3>	m_lastOmega;

	Timer::TimerInt	m_lastTime;

};
