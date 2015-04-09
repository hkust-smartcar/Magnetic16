/*
 * MyMotor.cpp
 *
 *  Created on: Mar 6, 2015
 *      Author: Peter
 */

#include <cassert>

#include <libsc/config.h>
#include <libsc/system.h>
#include <libsc/alternate_motor.h>
#include <libsc/dir_motor.h>
#include <libsc/ab_encoder.h>

#include "MyMotor.h"
#include "MyConfig.h"
#include "MyVar.h"
#include "MyLoop.h"
#include "MySmartCar.h"

using namespace libsc;
using namespace libbase::k60;

MyMotor *m_motorInstance;

inline void regularDecreasing(float &f, float amount, float min)
{
	f = (f - amount < min)? min : f - amount;
}

AlternateMotor::Config getAlterMotorConfig(const uint8_t id)
{
	AlternateMotor::Config config;
	config.id = id;
	return config;
}

DirMotor::Config getDirMotorConfig(const uint8_t id)
{
	DirMotor::Config config;
	config.id = id;
	return config;
}

AbEncoder::Config getEncoderConfig(const uint8_t id)
{
	AbEncoder::Config config;
	config.id = id;
	return config;
}

//#ifdef LIBSC_USE_ENCODER

MyMotor::MyEncoder::MyEncoder(MyConfig &config, MyVar &vars)
:
	m_myEncoder(getEncoderConfig(config.MyEncoderId)),
	m_lastCount(0)
{
	vars.lastCount = &m_lastCount;
}

int32_t MyMotor::MyEncoder::getEncoderReading(void)
{

	return getEncoderCount();
}

int32_t MyMotor::MyEncoder::getLastCount(void)
{
	return m_lastCount;
}

void MyMotor::MyEncoder::reset(void)
{
	m_myEncoder.Update();
	m_myEncoder.Update();
}

int32_t MyMotor::MyEncoder::getEncoderCount(void)
{
	m_myEncoder.Update();
	m_lastCount = m_myEncoder.GetCount();
	return m_lastCount;
}

//#endif

//#ifdef LIBSC_USE_MOTOR

MyMotor::MyMotor(MyConfig &config, MyVar &vars, MyLoop &loop)
:
//#ifdef LIBSC_MOTOR0_DIRMOTOR
	m_motor(getDirMotorConfig(config.MyMotorId)),
//#elif LIBSC_MOTOR0_ALTERMOTOR
//	m_motor(getAlterMotorConfig(config.MyMotorId)),
//#endif
	m_speedController(&config.MyMotorSpeedControlRef, &config.MyMotorSpeedControlKp, &config.MyMotorSpeedControlKi, &config.MyMotorSpeedControlKd, MIN_MOTOR_POWER, MAX_MOTOR_POWER),
	m_lastProcessSpeedControlTime(0),
	m_encoder(config, vars),
	m_speed(0),
	m_isStarted(false),
	m_isPushed(false),
	m_stopDelayTime(0),
	m_targetEncoderCount(&config.MyMotorSpeedControlRef),
	m_powerMode(&config.MySmartCarPowerMode)
{
	vars.speed = &m_speed;
	vars.isServoStarted = &m_isStarted;
	m_motorInstance = this;
	loop.addFunctionToLoop(&speedControlRoutine, LOOP_IMMEDIATELY, 10);
}

void MyMotor::setEnabled(const bool enabled)
{
	reset();
	m_isStarted = enabled;
}
bool MyMotor::isEnabled(void)
{
	return m_isStarted;
}

void MyMotor::reset(void)
{
	setSpeed(0);
	m_speedController.reset();
	m_encoder.reset();
	m_isStarted = false;
}

void MyMotor::speedControlRoutine(void)
{
	if (m_motorInstance->m_isStarted)
	{
		m_motorInstance->m_encoder.getEncoderReading();
		// RcCarMode has no effect
		if (*(m_motorInstance->m_powerMode) == MyConfig::SmartCarPowerMode::kLowFrictionMode)
		{
			if (!m_motorInstance->m_isPushed && m_motorInstance->m_encoder.getLastCount() >= *(m_motorInstance->m_targetEncoderCount))
			{
				*(m_motorInstance->m_targetEncoderCount) = inRange(0, m_motorInstance->m_encoder.getLastCount() * 3.0f, 70);
				return ;
			}
			else
			{
				m_motorInstance->m_isPushed = true;
				regularDecreasing(*(m_motorInstance->m_targetEncoderCount), 0.05f, 0.0f);
				if (*(m_motorInstance->m_targetEncoderCount) == 0.0f)
					if (++m_motorInstance->m_stopDelayTime > 100)
					{
						m_motorInstance->m_stopDelayTime = 0;
						m_motorInstance->m_isPushed = false;
					}
			}
		}
		else
			m_motorInstance->setSpeed((int16_t)m_motorInstance->m_speedController.updatePID((float)m_motorInstance->m_encoder.getLastCount()));
	}
	else
		m_motorInstance->setSpeed(0);
}

void MyMotor::setSpeed(const int16_t speed)
{
	if (speed > 0)
		m_motor.SetClockwise(true);
	else
		m_motor.SetClockwise(false);
	m_speed = speed;
	m_motor.SetPower(abs(m_speed));
}

//#endif
