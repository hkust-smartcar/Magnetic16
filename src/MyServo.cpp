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
#include <libsc/system.h>
#include <libsc/trs_d05.h>
#include <libbase/k60/adc.h>
#include <libsc/led.h>

#include "MyKalmanFilter.h"
#include "MyServo.h"
#include "PIDhandler.h"
#include "MyConfig.h"
#include "MyVar.h"
#include "MyLoop.h"
#include "MySmartCar.h"

using namespace libsc;
using namespace libbase::k60;
using namespace std;

MyServo *m_servoInstance;

inline uint8_t increaseInRange(uint8_t *v, uint8_t m, uint8_t x)
{
	*v = (*v + 1 >= x)? 0 : *v + 1;
	return *v;
}
inline uint8_t decreaseInRange(uint8_t *v, uint8_t m, uint8_t x)
{
	*v = ((int8_t)*v - 1 < x)? x - 1 : *v - 1;
	return *v;
}

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
	m_MagSenRawValue[0] = m_MagSens[0].GetResultF();
	m_MagSenRawValue[1] = m_MagSens[1].GetResultF();
	m_MagSenFilteredValue[0] = m_MagSenFilters[0].Filter(m_MagSenRawValue[0]);
	m_MagSenFilteredValue[1] = m_MagSenFilters[1].Filter(m_MagSenRawValue[1]);
	m_MagSenOffsetPrediction = (m_MagSenFilteredValue[0] - m_MagSenFilteredValue[1]) / *MaxSenReadingDiff * *DistanceWhenMaxDiff;
}

float MyServo::MyMagSenPair::getMagSenDiff(void)
{
	return m_MagSenFilteredValue[0] - m_MagSenFilteredValue[1];
}

float MyServo::MyMagSenPair::getMagSenSum(void)
{
	return m_MagSenFilteredValue[0] + m_MagSenFilteredValue[1];
}

float MyServo::MyMagSenPair::getMagSenPerentage(void)
{
	return getMagSenDiff() / getMagSenSum();
}

float *MyServo::MyMagSenPair::getFilteredLeftReadingPointer(void)
{
	return &m_MagSenFilteredValue[0];
}

float *MyServo::MyMagSenPair::getFilteredRightReadingPointer(void)
{
	return &m_MagSenFilteredValue[1];
}

float *MyServo::MyMagSenPair::getRawLeftReadingPointer(void)
{
	return &m_MagSenRawValue[0];
}

float *MyServo::MyMagSenPair::getRawRightReadingPointer(void)
{
	return &m_MagSenRawValue[1];
}

float MyServo::MyMagSenPair::operator[](size_t index)
{
	return m_MagSenFilteredValue[index];
}

float *MyServo::MyMagSenPair::getDistanceFromWire(void)
{
	return &m_MagSenOffsetPrediction;
}

MyServo::MyMagSenPair::MyMagSenPair(const uint8_t id, MyConfig &config)
:
	m_MagSens({ Adc(getAdcConfig(id + 0)), \
				Adc(getAdcConfig(id + 1))}),
	m_MagSenFilteredValue({0.0f,
						   0.0f}),
	m_MagSenFilters({ MyKalmanFilter(&config.MyMagSenFilterQ, &config.MyMagSenFilterR, 0.5f, 0.5f), \
					  MyKalmanFilter(&config.MyMagSenFilterQ, &config.MyMagSenFilterR, 0.5f, 0.5f)}),
	DistanceWhenMaxDiff(&config.MyMagSenDistanceWhenMaxDiff),
	MaxSenReadingDiff(&config.MyMagMaxSenReadingDiff)
{}

//#endif

MyServo::MyLeds::MyLeds(MyConfig &config)
:
	m_leds({ Led({0, true}), Led({1, true}), Led({2, true}), Led({3, true}) })
{}

Led &MyServo::MyLeds::operator[](size_t index)
{
	return this->m_leds[index];
}

void MyServo::MyLeds::turnAllOff(void)
{
	for (uint8_t i = 0; i < 4; i++)
		this->m_leds[i].SetEnable(false);
}

void MyServo::MyLeds::turnOffOtherAndSetEnabled(uint8_t id)
{
	for (uint8_t i = 0; i < 4; i++)
		if (i != id)
			this->m_leds[i].SetEnable(false);
	this->m_leds[id].SetEnable(true);
}

//#ifdef LIBSC_USE_SERVO

MyServo::MyServo(MyConfig &config, MyVar &vars, MyLoop &loop)
:
//#ifdef LIBSC_USE_MAGSEN
	m_MagSenSD(config.MyMagSenPairId0, config),
	m_MagSenFD(config.MyMagSenPairId1, config),
	m_MagSenHD(config.MyMagSenPairId2, config),
//	m_maxSDValue(config.MyServoDefaultMaxSDValue),
//	m_maxFDValue(config.MyServoDefaultMaxFDValue),
//	m_maxLRValue(config.MyServoDefaultMaxLRValue),
	m_referenceReading(0.0f),

	diffResultSD(0.0f),
	sumResultSD(0.0f),
	diffResultFD(0.0f),
	sumResultFD(0.0f),
	diffResultHD(0.0f),
	sumResultHD(0.0f),
	diffHDLocked(false),
	m_isStarted(false),
	usedMagSenPairs(config.MyMagSenPairCount),
//#endif
	m_leds(config),
	m_turningState(config.MySmartCarTurningMode),
	m_positionState(config.MySmartCarInitialPosition),
	m_currentStatus(config.MySmartCarInitialStatus),
	m_servo(getServoConfig(config.MyServoId)),
	m_turningController(&config.MyServoTurningRef, &config.MyServoTurningKp, &config.MyServoTurningKi, &config.MyServoTurningKd, -MAX_SERVO_TURNING_ANGLE, MAX_SERVO_TURNING_ANGLE),
	m_lastProcessTurningControlTime(System::Time()),
	m_setAngleList({ 0 }),
	m_lastTurningAngle(0),
	m_lastAngleListSum(0),
	m_lastAngleListIndex(0),
	m_hasItem(false),
	m_CrossRoadKp(config.MyServoCrossRoadKp),
	m_90DegreeKp(config.MyServo90DegreeKp),
	m_SDHighValueThreshold(config.MyMagSenSDStrongValue),
	m_SDNoSignalThreshold(config.MyMagSenSDWeakValue),
	m_HDNoSignalThreshold(config.MyMagSenHDWeakValue),
	m_HDHighValueThreshold(config.MyMagSenHDStrongValue),
	m_FDHighValueThreshold(config.MyMagSenFDStrongValue),
	m_FDLowValueThreshold(config.MyMagSenFDWeakValue)
{
	m_servoInstance = this;
	reset();
	vars.MagSenSDLeft = m_MagSenSD.getFilteredLeftReadingPointer();
	vars.MagSenSDRight = m_MagSenSD.getFilteredRightReadingPointer();
	vars.MagSenFDLeft = m_MagSenFD.getFilteredLeftReadingPointer();
	vars.MagSenFDRight = m_MagSenFD.getFilteredRightReadingPointer();
	vars.MagSenHDLeft = m_MagSenHD.getFilteredLeftReadingPointer();
	vars.MagSenHDRight = m_MagSenHD.getFilteredRightReadingPointer();
	vars.MagSenSDRawLeft = m_MagSenSD.getRawLeftReadingPointer();
	vars.MagSenSDRawRight = m_MagSenSD.getRawRightReadingPointer();
	vars.diffResultSD = &diffResultSD;
	vars.sumResultSD = &sumResultSD;
	vars.diffResultFD = &diffResultFD;
	vars.sumResultFD = &sumResultFD;
	vars.diffResultLR = &diffResultHD;
	vars.sumResultHD = &sumResultHD;
	vars.isMotorStarted = &m_isStarted;
	vars.lastTurningAngle = &m_lastTurningAngle;
	vars.magSenDefaultMaxSDValue = &m_maxSDValue;
	vars.magSenDefaultMaxFDValue = &m_maxFDValue;
	vars.magSenDefaultMaxHDValue = &m_maxLRValue;
	vars.magSenReferenceReading = &m_referenceReading;
	vars.lastAngleListSum = &m_lastAngleListSum;
	vars.PositionState = &m_positionState;
	vars.TurningState = &m_turningState;
	vars.CurrentStatus = &m_currentStatus;
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
	m_leds.turnAllOff();
}

void MyServo::turn(const int16_t degree_x10)
{
	int16_t realAngle = outRangeOf(inRange(-MAX_SERVO_TURNING_ANGLE, degree_x10, MAX_SERVO_TURNING_ANGLE), 0, 60);
//	m_servo.SetDegree(MID_SERVO_DEGREE + realAngle);
//	m_lastTurningAngle = realAngle;
	m_servo.SetDegree(MID_SERVO_DEGREE + getNextAngle());
	dropDownLastAngle(realAngle);
}

bool MyServo::isNoSignal(void)
{
	return (m_MagSenSD[0] < m_referenceReading && \
			m_MagSenSD[1] < m_referenceReading && \
			m_MagSenFD[0] < m_referenceReading && \
			m_MagSenFD[1] < m_referenceReading && \
			m_MagSenHD[0] < m_referenceReading && \
			m_MagSenHD[1] < m_referenceReading);
}

//bool MyServo::isCrossRoad(void)
//{
//	return (m_MagSenFD[1] > m_MagSenSD.getFilteredLeftReading() || \
//			m_MagSenFD[1] > m_MagSenSD.getFilteredRightReading() || \
//			m_MagSenFD[1] > m_MagSenSD.getFilteredLeftReading() || \
//			m_MagSenFD[1] > m_MagSenSD.getFilteredRightReading());
//}

void MyServo::getMagSenRange(const Timer::TimerInt interval)
{
	Timer::TimerInt startTime = System::Time();
	Timer::TimerInt lastTime = System::Time();

	float *SDL = m_MagSenSD.getFilteredLeftReadingPointer();
	float *SDR = m_MagSenSD.getFilteredRightReadingPointer();
	float *FDL = m_MagSenFD.getFilteredLeftReadingPointer();
	float *FDR = m_MagSenFD.getFilteredRightReadingPointer();
	float *LRL = m_MagSenHD.getFilteredLeftReadingPointer();
	float *LRR = m_MagSenHD.getFilteredRightReadingPointer();

	while (Timer::TimeDiff(System::Time(), startTime) < interval)
	{
		if (Timer::TimeDiff(System::Time(), lastTime) > 2)
		{
			m_MagSenSD.processReading();
			m_MagSenFD.processReading();
			m_MagSenHD.processReading();
			m_maxSDValue = max(m_maxSDValue, (float)max(*SDL, *SDR));
			m_maxFDValue = max(m_maxFDValue, (float)max(*FDL, *FDR));
			m_maxLRValue = max(m_maxLRValue, (float)max(*LRL, *LRR));
			lastTime = System::Time();
		}
		m_referenceReading = max(max(m_maxSDValue, m_maxFDValue), m_maxLRValue);
	}
}

void MyServo::dropDownLastAngle(const int16_t degree_x10)
{
	m_lastTurningAngle = degree_x10;
	m_setAngleList[m_lastAngleListIndex] = m_lastTurningAngle;
	m_lastAngleListIndex = increaseInRange(&m_lastAngleListIndex, 1, 20);
}

MyConfig::SmartCarPosition MyServo::getPosState(void)
{
	return m_positionState;
}

MyServo *MyServo::getServoInstance(void)
{
	return m_servoInstance;
}

int16_t MyServo::getNextAngle(void)
{
	if (m_lastAngleListIndex < 10)
		return m_setAngleList[10 + m_lastAngleListIndex];
	else
		return m_setAngleList[m_lastAngleListIndex - 10];
}

MyServo::LastTurningDirection MyServo::updateLastTurningAngle(void)
{
	m_lastAngleListSum = 0;
	for (uint8_t i = 0; i < length(m_setAngleList); i++)
		m_lastAngleListSum += m_setAngleList[i];
	return ((m_lastAngleListSum > 0)? LastTurningDirection::LEFT : ((m_lastAngleListSum < 0)? LastTurningDirection::RIGHT : LastTurningDirection::FORWARD));
}

void MyServo::updateLastState(void)
{
	if ((m_MagSenSD[MyMagSenPair::LEFT] < m_SDNoSignalThreshold && m_MagSenSD[MyMagSenPair::RIGHT] < m_SDNoSignalThreshold) ||
		(m_MagSenHD[MyMagSenPair::LEFT] < m_HDNoSignalThreshold || m_MagSenHD[MyMagSenPair::RIGHT] < m_HDNoSignalThreshold))
	{
		m_currentStatus = MyConfig::SmartCarStatus::kGGed;
		updateControlInstruction(MyServo::Functions::turnWithMaxAngle, false, MyConfig::SmartCarTurning::kNone, MyConfig::SmartCarStatus::kGGed);
		//diffHDLocked = true;
	}
	else
		m_currentStatus = MyConfig::SmartCarStatus::kNormal;

	if ((m_MagSenHD[MyMagSenPair::LEFT] > m_HDHighValueThreshold && m_MagSenHD[MyMagSenPair::RIGHT] < m_HDHighValueThreshold) ||
		(m_MagSenHD[MyMagSenPair::LEFT] < m_HDHighValueThreshold && m_MagSenHD[MyMagSenPair::RIGHT] < m_HDNoSignalThreshold))
		m_positionState = MyConfig::SmartCarPosition::kRight;
	else if ((m_MagSenHD[MyMagSenPair::RIGHT] > m_HDHighValueThreshold && m_MagSenHD[MyMagSenPair::LEFT] < m_HDHighValueThreshold) ||
			 (m_MagSenHD[MyMagSenPair::RIGHT] < m_HDHighValueThreshold && m_MagSenHD[MyMagSenPair::LEFT] < m_HDNoSignalThreshold))
		m_positionState = MyConfig::SmartCarPosition::kLeft;
	else
		m_positionState = MyConfig::SmartCarPosition::kIdeal;

	if (m_MagSenFD[MyMagSenPair::LEFT] > m_FDHighValueThreshold && m_MagSenFD[MyMagSenPair::RIGHT] > m_FDHighValueThreshold)
	{
		m_turningState = MyConfig::SmartCarTurning::kCrossRoad;
		updateControlInstruction(MyServo::Functions::turnAccordingToMagSenHD, false, MyConfig::SmartCarTurning::kStraightLine, MyConfig::SmartCarStatus::kNone);
	}
	else if (((m_MagSenFD[MyMagSenPair::LEFT] < m_FDLowValueThreshold && m_MagSenFD[MyMagSenPair::RIGHT] > m_FDHighValueThreshold) ||
			 (m_MagSenFD[MyMagSenPair::RIGHT] < m_FDLowValueThreshold && m_MagSenFD[MyMagSenPair::LEFT] > m_FDHighValueThreshold)) &&
			 ((m_MagSenSD[MyMagSenPair::LEFT] > m_SDHighValueThreshold && m_MagSenSD[MyMagSenPair::RIGHT] < m_SDNoSignalThreshold) ||
			  (m_MagSenSD[MyMagSenPair::RIGHT] > m_SDHighValueThreshold && m_MagSenSD[MyMagSenPair::LEFT] < m_SDNoSignalThreshold)))
	{
		m_turningState = MyConfig::SmartCarTurning::k90Degree;
		updateControlInstruction(MyServo::Functions::turnAccordingToMagSenFD, true, MyConfig::SmartCarTurning::kStraightLine, MyConfig::SmartCarStatus::kNone);
	}
	else
		m_turningState = MyConfig::SmartCarTurning::kStraightLine;
}

void MyServo::processControlQueue(void)
{
	if (m_hasItem)
		if ((m_specialControl.stopUntilType == m_turningState && !m_specialControl.determinedByStatus) || (m_specialControl.runWhileStatus != m_currentStatus && m_specialControl.determinedByStatus))
		{
			removeControlInstruction();
			diffHDLocked = false;
		}
		else
		{
			switch (m_specialControl.functionIndex)
			{
			case turnAccordingToMagSenHD:
				turn((int16_t)-diffResultHD * m_CrossRoadKp);
				break;

			case turnAccordingToMagSenFD:
				turn((int16_t)diffResultFD * m_90DegreeKp);
				break;

			case turnWithMaxAngle:
				turn((int16_t)(diffResultHD > 0)? -MAX_SERVO_TURNING_ANGLE : MAX_SERVO_TURNING_ANGLE);
				break;
			}
		}
	else
		turn((int16_t)m_turningController.updatePID_ori(-diffResultSD * 0.35 + diffResultHD * 0.65));
}

void MyServo::updateControlInstruction(const /*int16_t targetAngle*/Functions functionIndex, const bool allowChange, const MyConfig::SmartCarTurning stopUntil, const MyConfig::SmartCarStatus runWhile)
{
	if (!(m_specialControl.canBeChanged && m_hasItem))
	{
		m_hasItem = true;
//		m_specialControl.targetAngle = targetAngle;
		m_specialControl.functionIndex = functionIndex;
		if (stopUntil == MyConfig::SmartCarTurning::kNone && runWhile == MyConfig::SmartCarStatus::kNone)
			return ;
		else if (stopUntil != MyConfig::SmartCarTurning::kNone)
		{
			m_specialControl.stopUntilType = stopUntil;
			m_specialControl.determinedByStatus = false;
		}
		else
		{
			m_specialControl.runWhileStatus = runWhile;
			m_specialControl.determinedByStatus = true;
		}

		m_specialControl.canBeChanged = allowChange;
	}
}

void MyServo::removeControlInstruction(void)
{
	m_hasItem = false;
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
		if (m_servoInstance->usedMagSenPairs >= 3 && !m_servoInstance->diffHDLocked)
		{
			m_servoInstance->m_MagSenHD.processReading();
			m_servoInstance->diffResultHD = m_servoInstance->m_MagSenHD.getMagSenDiff();
		}
//		m_servoInstance->turningDispatch(m_servoInstance->diffResultSD * MAX_SERVO_TURNING_DEGREE);

		m_servoInstance->updateLastState();

		m_servoInstance->processControlQueue();

//		m_servoInstance->applyResult();

//		m_servoInstance->turn((int16_t)m_servoInstance->m_turningController.updatePID_ori(-m_servoInstance->diffResultSD));
	}
}

//#endif
