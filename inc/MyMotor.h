/*
 * MyMotor.h
 *
 *  Created on: Apr 25, 2015
 *      Author: Peter
 */

#pragma once

#include <libsc/dir_motor.h>
#include "MyResource.h"
#include "MyEncoder.h"
#include "MyLoop.h"
#include "PIDhandler.h"

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

	void setEnabled(const bool enabled);
	bool isEnabled(void);

	void reset(void);

	static void speedControlRoutine(void);

private:

	int16_t				m_speed;
	bool				m_enabled;
	PIDhandler			m_speedPID;

	static MyMotor		*m_instance;

};
