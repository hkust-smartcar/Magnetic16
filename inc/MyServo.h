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

	struct DataCollector_90degree
	{
		float				prevSd;
		float				prevFd;
		float				prevHd;
		Direction			turningDirection;
		bool				isFdReady;
		bool				isFdReachedLocMax;
		bool				isFdReachedLocMin;
		bool				isSdReady;
		bool				isSdReachedLocMax;
//		Timer::TimerInt		checkTime;
		bool				isReset;
	};

	MyServo(void);

	void reset(void);

	float getFinalAngle(void);
	bool check90Degree(void);
	void setAngle(const int16_t degree);
	void setDegree(const uint16_t degree);
	static void updateAngle(const uint32_t &timeDelay);

	void setEnabled(const bool enabled);
	bool isEnabled(void);

	bool					m_isPidNonLinear;
	uint8_t					m_90DegreeTurningNeed;
	float					m_lastError;
	uint16_t				m_lastDegree;
	Direction				m_lastTurningDirection;
	array<MyMagSen, 3>		m_MagSen;

private:

	array<MyPid, 2>			m_servoPid;

	array<float *, 3>		m_weight;

	bool					m_forceTurningFlag;

	bool					m_enabled;
	bool					m_pidChangedFlag;

	DataCollector_90degree	m_dataCollector_90;

	static MyServo			*m_instance;

};
