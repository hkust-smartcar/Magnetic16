/*
 * MyServo.cpp
 *
 *  Created on: Mar 6, 2015
 *      Author: Peter
 */

#include <vector>
#include <array>
#include <cassert>

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
		return LIBSC_MAGSEN2;
#endif

#if LIBSC_USE_MAGSEN > 3
	case 3:
		return LIBSC_MAGSEN3;
#endif

#if LIBSC_USE_MAGSEN > 4
	case 4:
		return LIBSC_MAGSEN0;
#endif

#if LIBSC_USE_MAGSEN > 5
	case 5:
		return LIBSC_MAGSEN1;
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

vector<Adc> getAdcPair(const uint8_t id)
{
	if (id % 2)
		assert(false);

	vector<Adc> newPair;
	for (uint8_t i = 0; i < 2; i++)
		newPair.push_back(Adc(getAdcConfig(id + i)));

	return newPair;
}

vector<MyKalmanFilter> MyServo::MyMagSenPair::getFilters(MyConfig &config)
{
	vector<MyKalmanFilter> newFilters;

//	newFilters.push_back(KalmanFilter(config.MyMagSenFilterQ, config.MyMagSenFilterR, m_MagSens[0].GetResultF(), 1.0f));
//	newFilters.push_back(KalmanFilter(config.MyMagSenFilterQ, config.MyMagSenFilterR, m_MagSens[1].GetResultF(), 1.0f));


	newFilters.push_back(MyKalmanFilter(&config.MyMagSenFilterQ, &config.MyMagSenFilterR, 0.5f, 0.5f));
	newFilters.push_back(MyKalmanFilter(&config.MyMagSenFilterQ, &config.MyMagSenFilterR, 0.5f, 0.5f));

	return newFilters;
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

float *MyServo::MyMagSenPair::getFilteredLeftReading(void)
{
	return &m_MagSenFilteredLeft;
}

float *MyServo::MyMagSenPair::getFilteredRightReading(void)
{
	return &m_MagSenFilteredRight;
}

float *MyServo::MyMagSenPair::getDistanceFromWire(void)
{
	return &m_MagSenOffsetPrediction;
}

MyServo::MyMagSenPair::MyMagSenPair(const uint8_t id, MyConfig &config)
:
	m_MagSens(getAdcPair(id)),
	m_MagSenFilteredLeft(0.0f),
	m_MagSenFilteredRight(0.0f),
	DistanceWhenMaxDiff(&config.MyMagSenDistanceWhenMaxDiff),
	MaxSenReadingDiff(&config.MyMagMaxSenReadingDiff)
{
	m_MagSenFilters = getFilters(config);
}

//#endif

//#ifdef LIBSC_USE_SERVO

MyServo::MyServo(MyConfig &config, MyVar &vars, MyLoop &loop)
:
//#ifdef LIBSC_USE_MAGSEN
	m_MagSenSD(config.MyMagSenPairId0, config),
	m_MagSenFD(config.MyMagSenPairId1, config),
	m_MagSenLR(config.MyMagSenPairId2, config),
	m_factorFD(&config.MyServoFactorSD),
	m_factorSD(&config.MyServoFactorFD),
	m_factorLR(&config.MyServoFactorLR),

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
	m_lastTurningAngle(0),
	m_lastTurningBeforeLowSignal(LastTurningDirection::FORWARD),
	m_lastFiveTimesTurningUpdateIndex(0)
{
	m_servoInstance = this;
	reset();
	vars.MagSenSDLeft = m_MagSenSD.getFilteredLeftReading();
	vars.MagSenSDRight = m_MagSenSD.getFilteredRightReading();
	vars.MagSenFDLeft = m_MagSenFD.getFilteredLeftReading();
	vars.MagSenFDRight = m_MagSenFD.getFilteredRightReading();
	vars.MagSenLRLeft = m_MagSenLR.getFilteredLeftReading();
	vars.MagSenLRRight = m_MagSenLR.getFilteredRightReading();
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
}

void MyServo::turnLeft(const uint16_t degree_x10)
{
	m_servo.SetDegree(MID_SERVO_DEGREE + inRange(MIN_SERVO_TURNING_DEGREE, degree_x10, MAX_SERVO_TURNING_DEGREE));
	dropDownLastAngle();
}

void MyServo::turnRight(const uint16_t degree_x10)
{
	m_servo.SetDegree(MID_SERVO_DEGREE - inRange(MIN_SERVO_TURNING_DEGREE, degree_x10, MAX_SERVO_TURNING_DEGREE));
	dropDownLastAngle();
}

void MyServo::turn(const int16_t degree_x10)
{
	m_servo.SetDegree(MID_SERVO_DEGREE + outRangeOf(inRange(-MAX_SERVO_TURNING_DEGREE, degree_x10, MAX_SERVO_TURNING_DEGREE), 0, 10));
	dropDownLastAngle();
}

void MyServo::dropDownLastAngle(void)
{
	m_lastTurningAngle = m_servo.GetDegree() - MID_SERVO_DEGREE;
	m_lastFiveTimesTurning[m_lastFiveTimesTurningUpdateIndex++] = m_lastTurningAngle;
	if (m_lastFiveTimesTurningUpdateIndex > m_lastFiveTimesTurning.size())
		m_lastFiveTimesTurningUpdateIndex = 0;
}

void MyServo::updateLastTurningAngle(void)
{
	int32_t sumOfAngle = 0;
	for (Byte i = 0; i < m_lastFiveTimesTurning.size(); i++)
		sumOfAngle += m_lastFiveTimesTurning[i];
	m_lastTurningBeforeLowSignal = (sumOfAngle > 0)? LastTurningDirection::LEFT : LastTurningDirection::RIGHT;
}

void MyServo::turningControlRoutine(void)
{
	if (m_servoInstance->usedMagSenPairs >= 1)
	{
		m_servoInstance->m_MagSenSD.processReading();
		m_servoInstance->diffResultSD = m_servoInstance->m_MagSenSD.getMagSenDiff();
		m_servoInstance->sumResultSD = m_servoInstance->m_MagSenSD.getMagSenSum();
	}
	if (m_servoInstance->usedMagSenPairs >= 2)
	{
		m_servoInstance->m_MagSenFD.processReading();
		m_servoInstance->diffResultFD = m_servoInstance->m_MagSenFD.getMagSenDiff();
		m_servoInstance->sumResultFD = m_servoInstance->m_MagSenSD.getMagSenSum();
	}
	if (m_servoInstance->usedMagSenPairs >= 3)
	{
		m_servoInstance->m_MagSenLR.processReading();
		m_servoInstance->diffResultLR = m_servoInstance->m_MagSenLR.getMagSenDiff();
		m_servoInstance->sumResultLR = m_servoInstance->m_MagSenSD.getMagSenSum();
	}
	//m_servoInstance->turn((int16_t)m_servoInstance->m_TurningController.updatePID(m_servoInstance->diffResultSD / m_servoInstance->sumResultSD));
	m_servoInstance->turn((int16_t)(m_servoInstance->diffResultSD * 1000));
}

MyServo::LastTurningDirection MyServo::getLastCarState(void)
{
	updateLastTurningAngle();
	return m_lastTurningBeforeLowSignal;
}

//#endif
