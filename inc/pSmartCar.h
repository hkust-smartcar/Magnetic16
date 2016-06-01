/*
 *	pSmartCar.h
 *	
 *	Author: petel__
 *	Copyright (c) 2014-2016 HKUST SmartCar Team
 *	Refer to LICENSE for details
 *	
 */

#include <array>
#include <functional>
#include <vector>
#include <libsc/system.h>
#include <pAngle.h>
#include <pMotor.h>
#include <libsc/mini_lcd.h>
#include <pFlash.h>
#include <pLoop.h>

using namespace libsc;
using namespace std;

#pragma once

class pSmartCar
{

public:

	struct State
	{
		float	dX;
		float	angle;
		float	dYaw;
		float	dAngle;

		Timer::TimerInt			m_lastUpdateTime;

		State(void)
		:
			dX(0.0f),
			angle(0.0f),
			dYaw(0.0f),
			dAngle(0.0f),
			m_lastUpdateTime(0)
		{}

		void update(void);
	};

	pSmartCar(void);

	void run(void);

protected:

	State					m_state;

	pLoop					m_loop;
	pAngle					m_angle;
	array<pMotor, 2>		m_motors;
	MiniLcd					m_lcd;

	static float leftMotorMapping(const float val);
	static float rightMotorMapping(const float val);

	static void update(void);
	void updateSensors(void);
	void balance(void);

};
