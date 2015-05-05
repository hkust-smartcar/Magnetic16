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

	array<MyMagSen, 3>		m_MagSen;

	MyServo(void);

	void reset(void);

	float getFinalAngle(void);
	float updateAngle(void);

	static void servoAngleRoutine(const uint32_t &timeDelay);

private:

	MyPID					m_servoPID;

	int16_t					m_lastAngle;

	array<float, 3>			m_weight;

	static MyServo			*m_instance;

};
