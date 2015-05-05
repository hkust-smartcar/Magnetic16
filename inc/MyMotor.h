/*
 * MyMotor.h
 *
 * Author: Peter
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <libsc/dir_motor.h>
#include "MyResource.h"
#include "MyEncoder.h"
#include "MyLoop.h"
#include "MyPID.h"

#define MIN_MOTOR_POWER	0
#define MAX_MOTOR_POWER	500

using namespace libsc;

class MyMotor : private DirMotor
{

public:

	MyEncoder	m_encoder;

	MyMotor(void);

	void setSpeed(int16_t speed);
	void updateSpeed(void);
	int16_t *getSpeed(void);

	void setEnabled(const bool enabled);
	bool isEnabled(void);

	void reset(void);

	static void speedControlRoutine(const uint32_t &timeDelay);

	MyPID				m_speedPID;

private:

	int16_t				m_speed;
	bool				m_enabled;

	static MyMotor		*m_instance;

};
