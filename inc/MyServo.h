/*
 * MyServo.h
 *
 *  Created on: Mar 6, 2015
 *      Author: Peter
 */

#pragma once

#include <array>

#include <libsc/config.h>
#include <libsc/trs_d05.h>
#include <libbase/k60/adc.h>
#include <libsc/system.h>
#include <libsc/led.h>

#include "MyKalmanFilter.h"
#include "PIDhandler.h"
#include "MyConfig.h"
#include "MyVar.h"
#include "MyLoop.h"

using namespace libsc;
using namespace libbase::k60;
using namespace std;

//#ifdef LIBSC_USE_SERVO

#define MID_SERVO_DEGREE 			900
#define MIN_SERVO_TURNING_DEGREE 	0
#define MAX_SERVO_TURNING_DEGREE	800

// cross road ->
//			  |
//
//SD L->1or>1 R<normal
//FD L>normal R>SDLor>1

// cross road ^
//			  |__
//
//oppsite to above

class MyServo
{

public:

	enum struct LastTurningDirection
	{
		FORWARD = 0,
		LEFT,
		RIGHT
	};

//#ifdef LIBSC_USE_MAGSEN
	class MyMagSenPair
	{

	public:

		enum WHICH
		{
			LEFT = 0,
			RIGHT
		};

		explicit MyMagSenPair(const uint8_t id, MyConfig &config);

		void processReading(void);

		float getMagSenDiff(void);
		float getMagSenSum(void);
		float getMagSenPerentage(void);

		float *getFilteredLeftReadingPointer(void);
		float *getFilteredRightReadingPointer(void);

		float operator[](size_t index);

		float *getDistanceFromWire(void);

	private:

		float					m_MagSenFilteredValue[2];
		float					m_MagSenOffsetPrediction;

		float					*DistanceWhenMaxDiff;
		float					*MaxSenReadingDiff;

		MyKalmanFilter			m_MagSenFilters[2];
		Adc						m_MagSens[2];

		MyKalmanFilter *getFilters(MyConfig &config);

	};

	class MyLeds
	{

	public:

		explicit MyLeds(MyConfig &config);

		Led &operator[](size_t index);

		void turnAllOff(void);
		void turnOffOtherAndSetEnabled(uint8_t id);

	private:

		Led						m_leds[4];

	};

//#endif

	explicit MyServo(MyConfig &config, MyVar &vars, MyLoop &loop);

	void setEnabled(const bool enabled);
	bool isEnabled(void);

	void reset(void);

	void turn(const int16_t degree_x10);

	bool isNoSignal(void);
	bool isCrossRoad(void);

	void getMagSenRange(const Timer::TimerInt interval);

	void dropDownLastAngle(const int16_t degree_x10);

	MyConfig::SmartCarPosition getPosState(void);

	static MyServo *getServoInstance(void);

	static void turningControlRoutine(void);

private:

	typedef struct AngleControlQueue
	{
		int16_t targetAngle;
		MyConfig::SmartCarTurning stopUntilType;
	};

	Timer::TimerInt		m_lastProcessTurningControlTime;
	int16_t				m_lastTurningAngle;

	float				m_maxSDValue;
	float				m_maxFDValue;
	float				m_maxLRValue;

//#ifdef LIBSC_USE_MAGSEN
	MyMagSenPair		m_MagSenSD;
	float 				diffResultSD;
	float 				sumResultSD;
	MyMagSenPair		m_MagSenFD;
	float 				diffResultFD;
	float 				sumResultFD;
	MyMagSenPair		m_MagSenHD;
	float 				diffResultHD;
	float 				sumResultHD;

	float				m_referenceReading;

	bool				m_isStarted;
	int16_t				m_setAngleList[20];
	uint8_t				m_lastAngleListIndex;
	int32_t				m_lastAngleListSum;

	float				m_HDNoSignalThreshold;
	float				m_HDHighValueThreshold;
	float				m_FDHighValueThreshold;

	array<AngleControlQueue, 5>	m_controlQueue;
	uint8_t				m_curQueueIndex;
	int8_t				m_controlQueueLength;

	uint8_t				usedMagSenPairs;
//#endif

	MyLeds				m_leds;
	MyConfig::SmartCarTurning 	m_turningState;
	MyConfig::SmartCarPosition	m_positionState;

	PIDhandler 			m_turningController;
	TrsD05				m_servo;


	int16_t getNextAngle(void);

	LastTurningDirection updateLastTurningAngle(void);

//	void turningDispatch(int16_t degree_x10);

	void updateLastState(void);

	void applyResult(void);

	void processControlQueue(void);

};

//#endif
