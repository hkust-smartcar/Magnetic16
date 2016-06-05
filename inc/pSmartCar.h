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
#include <libsc/mini_lcd.h>
#include <libutil/pGrapher.h>
#include <libsc/button.h>
#include <pAngle.h>
#include <pMotor.h>
#include <pFlash.h>
#include <pLoop.h>

#define isInRange(n, v, x) (v >= n && v <= x)

using namespace libsc;
using namespace std;

#pragma once

class pSmartCar
{

public:

	enum StatePos
	{
		prev = 0,
		cur
	};

	struct State
	{
		float	dX;
		float	angle;
		float	dYaw;
		float	dAngle;

		Timer::TimerInt	timeStamp;

		State(void)
		:
			dX(0.0f),
			angle(0.0f),
			dYaw(0.0f),
			dAngle(0.0f),
			timeStamp(0)
		{}

		State &operator=(const State &other);
	};

	pSmartCar(void);

	void run(void);

	State					m_state[2];

	pLoop					m_loop;
	pAngle					m_angle;
	array<pMotor, 2>		m_motors;
	MiniLcd					m_lcd;
	array<Button, 3>		m_buttons;
	pGrapher				m_grapher;

	void setMotorsEnabled(const bool enabled);

	static void onClickListener(const uint8_t id);

protected:

	bool					m_motorEnabled;

	static float leftMotorMapping(const float val);
	static float rightMotorMapping(const float val);

	static void update(void);
	static void speedControl(void);
	static void updateLcd(void);
	static void safetyCheck(void);

	void updateSensors(void);
	void updateState(void);
	void onDraw(void);

};
