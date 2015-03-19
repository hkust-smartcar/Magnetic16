/*
 * MyServo.cpp
 *
 *  Created on: Mar 6, 2015
 *      Author: Peter
 */

#include <array>
#include <cassert>
#include <stddef.h>
#include <utility>

#include <libsc/config.h>
#include <libsc/k60/system.h>
#include <libsc/k60/trs_d05.h>
#include <libbase/k60/adc.h>

#include "MyKalmanFilter.h"
#include "MyServo.h"
#include "PIDhandler.h"
#include "MyConfig.h"
#include "MyVar.h"
#include "MyLoop.h"
#include "MySmartCar.h"

using namespace libsc::k60;
using namespace libbase::k60;
using namespace std;

MyServo *m_servoInstance;

TrsD05::Config getServoConfig(const uint8_t id)
{
	TrsD05::Config config;
	config.id = id;
	return config;
}

//#ifdef LIBSC_USE_MAGSEN

inline Pin::Name GetPin(const uint8_t id)
{
	switch (id)
	{
	default:
		assert(false);
		// no break
#ifdef LIBSC_USE_MAGSEN
	case 0:
		return LIBSC_MAGSEN4;
#endif

#if LIBSC_USE_MAGSEN > 1
	case 1:
		return LIBSC_MAGSEN5;
#endif

#if LIBSC_USE_MAGSEN > 2
	case 2:
		return LIBSC_MAGSEN3;
#endif

#if LIBSC_USE_MAGSEN > 3
	case 3:
		return LIBSC_MAGSEN2;
#endif

#if LIBSC_USE_MAGSEN > 4
	case 4:
		return LIBSC_MAGSEN1;
#endif

#if LIBSC_USE_MAGSEN > 5
	case 5:
		return LIBSC_MAGSEN0;
#endif
	}
}

Adc::Config getAdcConfig(const uint8_t id)
{
	Adc::Config config;
	config.pin = GetPin(id);
	config.avg_pass = Adc::Config::AveragePass::k16;
	config.resolution = Adc::Config::Resolution::k8Bit;
	return config;
}

void MyServo::MyMagSenPair::processReading(void)
{
	m_MagSenFilteredLeft = m_MagSenFilters[0].Filter(m_MagSens[0].GetResultF());
	m_MagSenFilteredRight = m_MagSenFilters[1].Filter(m_MagSens[1].GetResultF());
	m_MagSenOffsetPrediction = (m_MagSenFilteredLeft - m_MagSenFilteredRight) / *MaxSenReadingDiff * *DistanceWhenMaxDiff;
}

float MyServo::MyMagSenPair::getMagSenDiff(void)
{
	return m_MagSenFilteredLeft - m_MagSenFilteredRight;
}

float MyServo::MyMagSenPair::getMagSenSum(void)
{
	return m_MagSenFilteredLeft + m_MagSenFilteredRight;
}

float *MyServo::MyMagSenPair::getFilteredLeftReadingPointer(void)
{
	return &m_MagSenFilteredLeft;
}

float *MyServo::MyMagSenPair::getFilteredRightReadingPointer(void)
{
	return &m_MagSenFilteredRight;
}

float MyServo::MyMagSenPair::getFilteredLeftReading(void)
{
	return m_MagSenFilteredLeft;
}

float MyServo::MyMagSenPair::getFilteredRightReading(void)
{
	return m_MagSenFilteredLeft;
}

float *MyServo::MyMagSenPair::getDistanceFromWire(void)
{
	return &m_MagSenOffsetPrediction;
}

MyServo::MyMagSenPair::MyMagSenPair(const uint8_t id, MyConfig &config)
:
	m_MagSens({ Adc(getAdcConfig(id + 0)), \
				Adc(getAdcConfig(id + 1)) }),
	m_MagSenFilteredLeft(0.0f),
	m_MagSenFilteredRight(0.0f),
	m_MagSenFilters({ MyKalmanFilter(&config.MyMagSenFilterQ, &config.MyMagSenFilterR, 0.5f, 0.5f), \
					  MyKalmanFilter(&config.MyMagSenFilterQ, &config.MyMagSenFilterR, 0.5f, 0.5f) }),
	DistanceWhenMaxDiff(&config.MyMagSenDistanceWhenMaxDiff),
	MaxSenReadingDiff(&config.MyMagMaxSenReadingDiff)
{}

//#endif

//#ifdef LIBSC_USE_SERVO

MyServo::MyServo(MyConfig &config, MyVar &vars, MyLoop &loop)
:
//#ifdef LIBSC_USE_MAGSEN
	m_MagSenSD(config.MyMagSenPairId0, config),
	m_MagSenFD(config.MyMagSenPairId1, config),
	m_MagSenLR(config.MyMagSenPairId2, config),
	m_minSDValue(&config.MyServoDefaultMinSDValue),
	m_maxSDValue(&config.MyServoDefaultMaxSDValue),
	m_minFDValue(&config.MyServoDefaultMinFDValue),
	m_maxFDValue(&config.MyServoDefaultMaxFDValue),
	m_minLRValue(&config.MyServoDefaultMinLRValue),
	m_maxLRValue(&config.MyServoDefaultMaxLRValue),

	diffResultSD(0.0f),
	sumResultSD(0.0f),
	diffResultFD(0.0f),
	sumResultFD(0.0f),
	diffResultLR(0.0f),
	sumResultLR(0.0f),
	m_isStarted(false),
	usedMagSenPairs(config.MyMagSenPairCount),
//#endif
	m_servo(getServoConfig(config.MyServoId)),
	m_TurningController(&config.MyServoTurningRef, &config.MyServoTurningKp, &config.MyServoTurningKi, &config.MyServoTurningKd, MIN_SERVO_TURNING_DEGREE, MAX_SERVO_TURNING_DEGREE),
	m_lastProcessTurningControlTime(System::Time()),
	m_setAngleList({ 0 }),
	m_lastTurningAngle(0),
	m_lastAngleListSum(0),
	m_lastAngleListIndex(0)
{
	m_servoInstance = this;
	reset();
	vars.MagSenSDLeft = m_MagSenSD.getFilteredLeftReadingPointer();
	vars.MagSenSDRight = m_MagSenSD.getFilteredRightReadingPointer();
	vars.MagSenFDLeft = m_MagSenFD.getFilteredLeftReadingPointer();
	vars.MagSenFDRight = m_MagSenFD.getFilteredRightReadingPointer();
	vars.MagSenLRLeft = m_MagSenLR.getFilteredLeftReadingPointer();
	vars.MagSenLRRight = m_MagSenLR.getFilteredRightReadingPointer();
	vars.diffResultSD = &diffResultSD;
	vars.sumResultSD = &sumResultSD;
	vars.diffResultFD = &diffResultFD;
	vars.sumResultFD = &sumResultFD;
	vars.diffResultLR = &diffResultLR;
	vars.sumResultLR = &sumResultLR;
	vars.isMotorStarted = &m_isStarted;
	vars.lastTurningAngle = &m_lastTurningAngle;
	vars.magSenFilterQ = &config.MyMagSenFilterQ;
	vars.magSenFilterR = &config.MyMagSenFilterR;
	vars.magSenDefaultMaxSDValue = m_maxSDValue;
	vars.magSenDefaultMinSDValue = m_minSDValue;
	vars.magSenDefaultMaxFDValue = m_maxFDValue;
	vars.magSenDefaultMinFDValue = m_minFDValue;
	vars.magSenDefaultMaxLRValue = m_maxLRValue;
	vars.magSenDefaultMinLRValue = m_minLRValue;
	loop.addFunctionToLoop(&turningControlRoutine, LOOP_IMMEDIATELY, LOOP_EVERYTIME);
}

void MyServo::setEnabled(const bool enabled)
{
	m_isStarted = enabled;
}
bool MyServo::isEnabled(void)
{
	return m_isStarted;
}

void MyServo::reset(void)
{
	m_servo.SetDegree(MID_SERVO_DEGREE);
	m_isStarted = false;
	m_lastAngleListIndex = 0;
	m_lastAngleListSum = 0;
	m_lastTurningAngle = 0;
}

void MyServo::turn(const int16_t degree_x10)
{
	int16_t realAngle = outRangeOf(inRange(-MAX_SERVO_TURNING_DEGREE, degree_x10, MAX_SERVO_TURNING_DEGREE), 0, 60);
	m_servo.SetDegree(MID_SERVO_DEGREE + getNextAngle());
	dropDownLastAngle(realAngle);
}

bool MyServo::isNoSignal(const float ref)
{
	return (m_MagSenSD.getFilteredLeftReading() < ref && \
			m_MagSenSD.getFilteredLeftReading() < ref && \
			m_MagSenFD.getFilteredLeftReading() < ref && \
			m_MagSenFD.getFilteredLeftReading() < ref && \
			m_MagSenLR.getFilteredLeftReading() < ref && \
			m_MagSenLR.getFilteredLeftReading() < ref);
}

bool MyServo::isCrossRoad(void)
{
	return (m_MagSenFD.getFilteredLeftReading() > m_MagSenSD.getFilteredLeftReading() || \
			m_MagSenFD.getFilteredLeftReading() > m_MagSenSD.getFilteredRightReading() || \
			m_MagSenFD.getFilteredRightReading() > m_MagSenSD.getFilteredLeftReading() || \
			m_MagSenFD.getFilteredRightReading() > m_MagSenSD.getFilteredRightReading());
}

void MyServo::getMagSenRange(const Timer::TimerInt interval)
{
	Timer::TimerInt startTime = System::Time();
	Timer::TimerInt lastTime = System::Time();

	float *SDL = m_MagSenSD.getFilteredLeftReadingPointer();
	float *SDR = m_MagSenSD.getFilteredRightReadingPointer();
	float *FDL = m_MagSenFD.getFilteredLeftReadingPointer();
	float *FDR = m_MagSenFD.getFilteredRightReadingPointer();
	float *LRL = m_MagSenLR.getFilteredLeftReadingPointer();
	float *LRR = m_MagSenLR.getFilteredRightReadingPointer();

	while (Timer::TimeDiff(System::Time(), startTime) < interval)
	{
		if (Timer::TimeDiff(System::Time(), lastTime) > 2)
		{
			m_MagSenSD.processReading();
			m_MagSenFD.processReading();
			m_MagSenLR.processReading();
			*m_maxSDValue = max(*m_maxSDValue, (float)((*SDL + *SDR) / 2));
			*m_minSDValue = min(*m_minSDValue, (float)((*SDL + *SDR) / 2));
			*m_maxFDValue = max(*m_maxFDValue, (float)((*FDL + *FDR) / 2));
			*m_minFDValue = min(*m_minFDValue, (float)((*FDL + *FDR) / 2));
			*m_maxLRValue = max(*m_maxLRValue, (float)((*LRL + *LRR) / 2));
			*m_minLRValue = min(*m_minLRValue, (float)((*LRL + *LRR) / 2));
			lastTime = System::Time();
		}
	}
}

void MyServo::dropDownLastAngle(const int16_t degree_x10)
{
	m_lastTurningAngle = degree_x10;
	m_setAngleList[m_lastAngleListIndex++] = m_lastTurningAngle;
	m_lastAngleListSum += m_lastTurningAngle;
}

int16_t MyServo::getNextAngle(void)
{
	uint8_t nextIndex = 0;
	if (m_lastAngleListIndex < 10)
		nextIndex = m_setAngleList[10 + m_lastAngleListIndex];
	else
		nextIndex = m_setAngleList[m_lastAngleListIndex - 10];

	m_lastAngleListSum -= m_setAngleList[nextIndex];
	return m_setAngleList[nextIndex];
}

MyServo::LastTurningDirection MyServo::updateLastTurningAngle(void)
{
	int32_t sumOfAngle = 0;
	for (uint8_t i = 0; i < length(m_setAngleList); i++)
		sumOfAngle += m_setAngleList[i];
	return ((sumOfAngle > 0)? LastTurningDirection::LEFT : LastTurningDirection::RIGHT);
}

void MyServo::turningDispatch(int16_t degree_x10)
{
	if (isCrossRoad())
		turn(0);
	else if (isNoSignal(*m_maxFDValue))
	{
		switch ((uint8_t)updateLastTurningAngle())
		{
		case (uint8_t)LastTurningDirection::FORWARD:
			turn(0);
			break;
		case (uint8_t)LastTurningDirection::LEFT:
			turn(MAX_SERVO_TURNING_DEGREE);
			break;
		case (uint8_t)LastTurningDirection::RIGHT:
			turn(-MAX_SERVO_TURNING_DEGREE);
			break;
		}
	}
	else
		turn(degree_x10);
}

void MyServo::turningControlRoutine(void)
{
	if (m_servoInstance->m_isStarted)
	{
		if (m_servoInstance->usedMagSenPairs >= 1)
		{
			m_servoInstance->m_MagSenSD.processReading();
			m_servoInstance->diffResultSD = m_servoInstance->m_MagSenSD.getMagSenDiff();
		}
		if (m_servoInstance->usedMagSenPairs >= 2)
		{
			m_servoInstance->m_MagSenFD.processReading();
			m_servoInstance->diffResultFD = m_servoInstance->m_MagSenFD.getMagSenDiff();
		}
		if (m_servoInstance->usedMagSenPairs >= 3)
		{
			m_servoInstance->m_MagSenLR.processReading();
			m_servoInstance->diffResultLR = m_servoInstance->m_MagSenLR.getMagSenDiff();
		}

		m_servoInstance->turningDispatch(m_servoInstance->diffResultSD * MAX_SERVO_TURNING_DEGREE);
	}
}

//#endif
