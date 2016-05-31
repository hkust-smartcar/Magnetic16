/*
 *	pAngle.h
 *	
 *	Author: petel__
 *	Copyright (c) 2014-2016 HKUST SmartCar Team
 *	Refer to LICENSE for details
 *	
 */

#pragma once

#include <cassert>
#include <array>
#include <cmath>
#include <libsc/mma8451q.h>
#include <libsc/mpu6050.h>
#include <libsc/timer.h>
#include <libsc/system.h>

using namespace libsc;

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

	bool isReady(void) const;

	void update(void);

	float getAngle(void) const;

private:

	Config			m_param;
	float			m_lastAngle;
	float			m_gyroAngle;
	float			m_gyroOffset;

	Timer::TimerInt	m_lastTime;

};
