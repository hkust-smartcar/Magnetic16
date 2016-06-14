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
#include <libsc/led.h>
#include <libutil/pGrapher.h>
#include <libutil/kalman_filter.h>
#include <libsc/simple_buzzer.h>
#include <libsc/battery_meter.h>
#include <libsc/button.h>
#include <pKalmanFilter.h>
#include <pAngle.h>
#include <pMotor.h>
#include <pFlash.h>
#include <pLoop.h>

#define ABS(v) ((v > 0)? v : -v)
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

	enum Type
	{
		Angle = 0,
		Direction,
		Speed
	};

	pSmartCar(void);

	void reset(void);

	void run(void);

	void setMotorsEnabled(const bool enabled);
	void setMotorPower(const uint8_t id, const int16_t power);
	void updateMotors(void);

	void setLed(const uint8_t index, const bool enabled);

	void sendDataToGrapher(void);

	static void onClickListener(const uint8_t id);
	int16_t set_accel;
		int16_t cur_accel_l;
		int16_t cur_accel_r;
		int16_t cur_speed_l;
		int16_t cur_speed_r;
		int16_t last_speed_l;
		int16_t last_speed_r;
		int16_t stop;

protected:

	pPid::PidParam getPidConfig(Type type);

	void updateSmoothAngleOutput(const int16_t speed);
	int16_t	getSmoothAngleOutput(void);

	void updatePid(const float val, Type type);

	void addAllRoutineToLoop(void);
	void addVariablesToGrapher(void);

	static float leftMotorMapping(const float val);
	static float rightMotorMapping(const float val);

	static void update(void);
	static void angleControl(void);
	static void directionControl(void);
	static void speedControl(void);
	static void print(void);
//	static void safetyCheck(void);

	void updateSensors(void);
	void updateState(void);
	void updateSpeed(void);
	void onDraw(void);

	State					m_state[2];

	float					m_direction;
	float					m_speed;
	float					m_idealAngleOffset;
	float					m_idealAngle;

	float					m_batteryVoltage;

	pLoop					m_loop;
	pAngle					m_angle;
	array<pMotor, 2>		m_motors;
	MiniLcd					m_lcd;
	array<Button, 3>		m_buttons;
	array<Led, 4>			m_leds;
	SimpleBuzzer			m_buzzer;
	BatteryMeter			m_batmeter;
	pGrapher				m_grapher;

	bool					m_motorEnabled;

	array<pPid, 3>			m_pidControllers;
	array<float, 3>			m_pidOutputVal;

	array<pKalmanFilter, 3>	m_filter;
	uint8_t					m_smoothCounter;
	float					m_smoothIncrement;

};
