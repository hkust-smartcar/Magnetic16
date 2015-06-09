/*
 * MyServo.h
 *
 *  Created on: Apr 25, 2015
 *      Author: Peter
 */

#pragma once

#include <array>
#include <libbase/k60/adc.h>
#include <libsc/trs_d05.h>
#include "MyMagSen.h"
#include "MyPID.h"
#include "MyResource.h"

#define MAX_SERVO_ANGLE 900
#define MIN_SERVO_ANGLE -MAX_SERVO_ANGLE
#define MID_SERVO_ANGLE 900

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

	array<MyMagSen, 3>		m_MagSen;

	MyServo(void);

	void reset(void);

	float getFinalAngle(void);
	void setDegree(const int16_t degree);
	void updateAngle(void);

	void setEnabled(const bool enabled);
	bool isEnabled(void);

	static void servoAngleRoutine(const uint32_t &timeDelay);

	float					m_lastAngle;
	uint16_t				m_lastDegree;
	Direction				m_lastTurningDirection;

private:

	MyPID					m_servoPID;

	array<float *, 3>		m_weight;

	bool					m_enabled;

	static MyServo			*m_instance;

};
