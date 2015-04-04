/*
 * MyMotor.h
 *
 *  Created on: Mar 6, 2015
 *      Author: Peter
 */

#pragma once


#include <cassert>

#include <libsc/alternate_motor.h>
#include <libsc/dir_motor.h>
#include <libsc/ab_encoder.h>
#include <libsc/config.h>

#include "PIDhandler.h"
#include "MyConfig.h"
#include "MyVar.h"
#include "MyLoop.h"
#include "PIDhandler.h"

using namespace libsc;
using namespace libbase::k60;

//#ifdef LIBSC_USE_MOTOR

#define MIN_MOTOR_POWER	0
#define MAX_MOTOR_POWER	500

class MyMotor
{

public:

//#ifdef LIBSC_USE_ENCODER
	class MyEncoder
	{

	public:

		explicit MyEncoder(MyConfig &config, MyVar &vars);

		int32_t getEncoderReading(void);

		int32_t getLastCount(void);

		void reset(void);

	private:

		int32_t getEncoderCount(void);

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

	MyConfig::SmartCarPowerMode	*m_powerMode;

	// Motor
	void setSpeed(const int16_t speed);

	MyEncoder			m_encoder;
	int16_t				m_speed;
	float				*m_targetEncoderCount;

	uint8_t				m_stopDelayTime;
	bool				m_isPushed;
	bool				m_isStarted;

//#ifdef LIBSC_MOTOR0_DIRMOTOR
	DirMotor			m_motor;
//#elif LIBSC_MOTOR0_ALTERMOTOR
//	AlternateMotor		m_motor;
//#endif

};

//#endif
