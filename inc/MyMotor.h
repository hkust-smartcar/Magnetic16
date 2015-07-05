/*
 * MyMotor.h
 *
 * Author: Peter
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <libsc/dir_motor.h>
#include <MyPid.h>
#include "MyResource.h"
#include "MyEncoder.h"
#include "MyLoop.h"

#define MIN_MOTOR_POWER	0
#define MAX_MOTOR_POWER	500

using namespace libsc;

class MyMotor : private DirMotor
{

public:

	MyEncoder	m_encoder;

	MyMotor(void);

	void setSpeed(int16_t speed);
	static void updateSpeed(const uint32_t &timeDelay);
	int16_t *getSpeed(void);

	void setEnabled(const bool enabled);
	bool isEnabled(void);

	void reset(void);

//	float				m_curReference;
	MyPid				m_speedPid;

private:

	uint32_t			m_timePassed;
	int16_t				m_speed;
	bool				m_enabled;

	static MyMotor		*m_instance;

};
