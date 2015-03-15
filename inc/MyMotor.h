/*
 * MyMotor.h
 *
 *  Created on: Mar 6, 2015
 *      Author: Peter
 */

#pragma once


#include <cassert>

#include <libsc/k60/alternate_motor.h>
#include <libsc/k60/dir_motor.h>
#include <libsc/k60/ab_encoder.h>
#include <libsc/config.h>

#include "PIDhandler.h"
#include "MyConfig.h"
#include "MyVar.h"
#include "MyLoop.h"
#include "PIDhandler.h"

//#ifdef LIBSC_USE_MOTOR

using namespace libsc::k60;
using namespace libbase::k60;

class MyMotor
{

public:

//#ifdef LIBSC_USE_ENCODER
	class MyEncoder
	{

	public:

		explicit MyEncoder(MyConfig &config, MyVar &vars);

		int32_t getEncoderReading(void);

		void reset(void);

	private:

		void getEncoderCount(void);

		int32_t				m_lastCount;

		AbEncoder			m_myEncoder;

	};
//#endif

	explicit MyMotor(MyConfig &config, MyVar &vars, MyLoop &loop);

	void setEnabled(const bool enabled);
	bool isEnabled(void);

	void reset(void);

	static void speedControlRoutine(void);

private:

	Timer::TimerInt		m_lastProcessSpeedControlTime;
	PIDhandler			m_speedController;

	// Motor
	void setSpeed(const int16_t speed);

	MyEncoder			m_encoder;
	int16_t				m_speed;

	bool				m_isStarted;

//#ifdef LIBSC_MOTOR0_DIRMOTOR
	DirMotor			m_motor;
//#elif LIBSC_MOTOR0_ALTERMOTOR
//	AlternateMotor		m_motor;
//#endif

};

//#endif
