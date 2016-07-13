/*
 *	pSmartCar.h
 *	
 *	Author: petel__
 *	Copyright (c) 2014-2016 HKUST SmartCar Team
 *	Refer to LICENSE for details
 *	
 */

#include <cassert>
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
#include <libsc/joystick.h>
#include <pKalmanFilter.h>
#include <pLowPassFilter.h>
#include <pAngle.h>
#include <pMagSen.h>
#include <pBuzzer.h>
#include <pMotor.h>
#include <pFlash.h>
#include <pLoop.h>
#include <pFuzzyLogic.h>

#define ABS(v) ((v > 0)? v : -v)
#define inRange(n, v, x) ((v < n)? n : ((v > x)? x : v))
#define inRange2(v, m, r) ((v < m - r)? m - r : ((v > m + r)? m + r : v))
#define inRange3(v, a, b) ((a >= b)? inRange(b, v, a) : inRange(a, v, b))
#define isInRange(n, v, x) (v >= n && v <= x)
#define isInRange2(v, m, r) ((v < m + r) && (v > m - r))
#define sgn(v) ((v > 0)? 1 : -1)

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
		Speed
	};

	enum IncrementType
	{
		SpeedIncrement = 0,
		DirectionIncrement
	};

	pSmartCar(void);

	void reset(void);

	void run(void);

	void addSpeed(const float speed);

	void setMotorsEnabled(const bool enabled);
	bool isMotorsEnabled(void);

	void setMotorPower(const uint8_t id, const int16_t power);
	void updateMotors(void);

	bool isReadyAndSet(void);

	void setLed(const uint8_t index, const bool enabled);
	void setBeep(const bool isBeep, const uint8_t noteIndex = 40);

	void sendDataToGrapher(void);

	static void onClickListener(const uint8_t id);

protected:

	pPid::PidParam getPidConfig(Type type);

	void updateSmoothAngleOutput(const float newAngle);
	float getSmoothAngleOutput(void);

	void updateSmoothDirectionOutput(const float newDirection);
	float getSmoothDirectionOutput(void);

	void updatePid(const float val, Type type);

	void addAllRoutineToLoop(void);
	void addVariablesToGrapher(void);

	static void watchDogTimeout(void);

	static float leftMotorMapping(const float val);
	static float rightMotorMapping(const float val);

	static void onReceive(const std::vector<Byte>& bytes);

	static void update(void);
	static void angleControl(void);
	static void selfDirectionControl(void);
	static void directionControl(void);
	static void speedControl(void);
	static void print(void);

	void smoothedIdealSpeed(const float &accelLimit);
	void updateSensors(void);
	void updateState(void);
	void updateSpeed(void);
	void onDraw(void);

	State					m_state[2];
	float					m_direction;
	float					m_idealSpeed;
	float					m_curSpeed;
	float					m_idealAngleOffset;
	float					m_directionOffset;
	float					m_idealAngle;

	float					m_batteryVoltage;

	pLoop					m_loop;
	pAngle					m_angle;
	array<pMotor, 2>		m_motors;
//	MiniLcd					m_lcd;
//	Joystick				m_joystick;
//	array<Button, 3>		m_buttons;
	array<Led, 4>			m_leds;
	pBuzzer					m_buzzer;
	array<pMagSen, 2>		m_magSen;
	BatteryMeter			m_batmeter;
	pGrapher				m_grapher;
	pLowPassFilter			m_encoderLpf;
	pFuzzyLogic				m_fuzzyLogic;

	bool					m_motorEnabled;
	bool					m_isReadyToRun;

	uint8_t 				m_ignoreSpeedCounter;

	array<pPid, 2>			m_pidControllers;
	array<float, 2>			m_pidOutputVal;

	uint8_t					m_smoothCounter;
	array<float, 2>			m_smoothIncrement;

};
