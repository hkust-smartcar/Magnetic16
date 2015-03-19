/*
 * MyServo.h
 *
 *  Created on: Mar 6, 2015
 *      Author: Peter
 */

#pragma once

#include <array>

#include <libsc/config.h>
#include <libsc/k60/trs_d05.h>
#include <libbase/k60/adc.h>
#include <libsc/k60/system.h>

#include "MyKalmanFilter.h"
#include "PIDhandler.h"
#include "MyConfig.h"
#include "MyVar.h"
#include "MyLoop.h"

using namespace libsc::k60;
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

		float *getFilteredLeftReadingPointer(void);
		float *getFilteredRightReadingPointer(void);
		float getFilteredLeftReading(void);
		float getFilteredRightReading(void);

		float *getDistanceFromWire(void);

	private:

		float					m_MagSenFilteredLeft;
		float					m_MagSenFilteredRight;
		float					m_MagSenOffsetPrediction;

		float					*DistanceWhenMaxDiff;
		float					*MaxSenReadingDiff;

		MyKalmanFilter			m_MagSenFilters[2];
		Adc						m_MagSens[2];

		MyKalmanFilter *getFilters(MyConfig &config);

	};

//#endif

	explicit MyServo(MyConfig &config, MyVar &vars, MyLoop &loop);

	void setEnabled(const bool enabled);
	bool isEnabled(void);

	void reset(void);

	void turn(const int16_t degree_x10);

	bool isNoSignal(const float ref);
	bool isCrossRoad(void);

	void getMagSenRange(const Timer::TimerInt interval);

	void dropDownLastAngle(const int16_t degree_x10);

	static void turningControlRoutine(void);

private:

	Timer::TimerInt		m_lastProcessTurningControlTime;
	int16_t				m_lastTurningAngle;

	float				*m_maxSDValue;
	float				*m_minSDValue;
	float				*m_maxFDValue;
	float				*m_minFDValue;
	float				*m_maxLRValue;
	float				*m_minLRValue;

//#ifdef LIBSC_USE_MAGSEN
	MyMagSenPair		m_MagSenSD;
	float 				diffResultSD;
	float 				sumResultSD;
	MyMagSenPair		m_MagSenFD;
	float 				diffResultFD;
	float 				sumResultFD;
	MyMagSenPair		m_MagSenLR;
	float 				diffResultLR;
	float 				sumResultLR;

	bool				m_isStarted;
	int16_t				m_setAngleList[20];
	int8_t				m_lastAngleListIndex;
	int32_t				m_lastAngleListSum;

	uint8_t				usedMagSenPairs;
//#endif


	PIDhandler 			m_TurningController;
	TrsD05				m_servo;


	int16_t getNextAngle(void);

	LastTurningDirection updateLastTurningAngle(void);

	void turningDispatch(int16_t degree_x10);

};

//#endif
