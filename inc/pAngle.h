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
#define inRange(n, v, x) ((v > x)? x : ((v < n)? n : v))

class pAngle : private Mma8451q, private Mpu6050
{

public:

	struct Config
	{
		uint16_t updatePeriod;
		float accelTrustValue;

		bool useStrict = true;

		Config(uint16_t _updatePeriod, float _accelTrustValue, bool _useStrict = true)
		:
			updatePeriod(_updatePeriod),
			accelTrustValue(_accelTrustValue),
			useStrict(_useStrict)
		{}
	};

	typedef enum Axis
	{
		x = 0,
		y,
		z
	};

	pAngle(Config config);

	void update(void);

	float getAngle(void) const;

	array<float, 3> getAccel(void) const;
	array<float, 3> getOmega(void) const;
	float getAccel(const uint8_t index) const;
	float getOmega(const uint8_t index) const;

private:

	Config			m_param;
	float			m_lastAngle;
	float			m_gyroAngle;
	float			m_gyroOffset;
	array<float, 3>	m_lastAccel;
	array<float, 3>	m_lastOmega;

	Timer::TimerInt	m_lastTime;

};
