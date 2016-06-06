/*
 *	pSmartCar.h
 *	
 *	Author: petel__
 *	Copyright (c) 2014-2016 HKUST SmartCar Team
 *	Refer to LICENSE for details
 *	
 */

#include <array>
#include <cmath>
#include <functional>
#include <vector>
#include <libsc/system.h>
#include <libsc/mini_lcd.h>
#include <libutil/pGrapher.h>
#include <libutil/kalman_filter.h>
#include <libsc/button.h>
#include <pAngle.h>
#include <pMotor.h>
#include <pFlash.h>
#include <pLoop.h>

#define inRange(n, v, x) ((v < n)? n : ((v > x)? x : v))
#define isInRange(n, v, x) (v >= n && v <= x)

using namespace libsc;
using namespace libutil;
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

	enum PidType
	{
		Angle = 0,
		Direction,
		Speed
	};

	pSmartCar(void);

	void reset(void);

	void run(void);

	void setMotorsEnabled(const bool enabled);

	static void onClickListener(const uint8_t id);

protected:

	pPid::PidParam getPidConfig(PidType type);

	void updateSmoothOutput(const int16_t speed);
	int16_t	getSmoothSpeedOutput(void);

	void updatePid(const float val, PidType type);

	void addAllRoutineToLoop(void);
	void addVariablesToGrapher(void);

	static float leftMotorMapping(const float val);
	static float rightMotorMapping(const float val);

	static void update(void);
	static void angleControl(void);
	static void directionControl(void);
	static void speedControl(void);
	static void print(void);
	static void safetyCheck(void);

	void updateSensors(void);
	void updateMotors(void);
	void updateState(void);
	void updateSpeed(void);
	void onDraw(void);

	State					m_state[2];

	float					m_direction;
	float					m_speed;

	pLoop					m_loop;
	pAngle					m_angle;
	array<pMotor, 2>		m_motors;
	MiniLcd					m_lcd;
	array<Button, 3>		m_buttons;
	pGrapher				m_grapher;

	bool					m_motorEnabled;

	array<pPid, 3>			m_pidControllers;
	array<int16_t, 3>		m_pidOutputVal;

	KalmanFilter			m_filter;
	float					m_oldSpeedPidOuput;
	uint8_t					m_smoothCounter;
	float					m_smoothIncrement;

};
