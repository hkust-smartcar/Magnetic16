/*
 * MyServo.cpp
 *
 * Author: Peter
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <array>
#include <libbase/k60/adc.h>
#include <libsc/timer.h>
#include <libsc/trs_d05.h>
#include <MyPid.h>
#include "MyMagSen.h"
#include "MyResource.h"

#define MID_SERVO_ANGLE 900
#define MID_SERVO_DEGREE 900

#define MAX_SERVO_ANGLE 500

#define MAX_SERVO_DEGREE 1400
#define MIN_SERVO_DEGREE 400

#define isInRange(n, v, x) ((v >= n) && (v <= x))

using namespace libsc;
using namespace libbase::k60;
using namespace std;

class MyServo : private TrsD05
{

public:

	enum Direction
	{
		LEFT = 0,
		RIGHT
	};

	enum PidType
	{
		Normal = 0,
		Turning
	};

	enum LastServoLockDirection
	{
		None = 0,
		Left,
		Right
	};

	struct Turning90DegreeChecker
	{
		Timer::TimerInt			lastTime;
		bool					hasReachThreshold;
		bool					is90DegreeTurningStarted;
		LastServoLockDirection	direction;
	};

	MyServo(void);

	void reset(void);

	void updateMagSen(void);
	float getFinalAngle(void);
	bool turningHandler(float &error);
	bool check90Degree(float &error);
	void setAngle(const int16_t degree);
	void setDegree(const uint16_t degree);
	static void updateAngle(const uint32_t &timeDelay);

	void setEnabled(const bool enabled);
	bool isEnabled(void);

	bool					m_allow90DegreeTurning;
	bool					m_isPidNonLinear;
	bool					m_90DegreeTurningNeed;
	float					m_lastError;
	int16_t					m_finalAngle;
	uint16_t				m_lastDegree;
	LastServoLockDirection	m_lastTurningDirection;
	array<MyMagSen, 3>		m_MagSen;

	array<MyPid, 2>			m_servoPid;

private:

//	array<float *, 3>		m_normal_weight;
//	array<float *, 3>		m_turning_weight;

	Turning90DegreeChecker	m_90DegreeChecker;

	bool					m_forceTurningFlag;

	bool					m_enabled;
	bool					m_pidChangedFlag;

	static MyServo			*m_instance;

};
