/*
 * MyVar.h
 *
 *  Created on: Mar 13, 2015
 *      Author: Peter
 */

#pragma once

#include "MyConfig.h"

class MyVar
{

public:

	// MyMotor
	int16_t				*speed;
	bool				*isMotorStarted;

	// MyEncoder
	int32_t				*lastCount;

	// MyMagSen
	float				*MagSenSDLeft;
	float				*MagSenSDRight;
	float				*MagSenFDLeft;
	float				*MagSenFDRight;
	float				*MagSenHDLeft;
	float				*MagSenHDRight;

	float 				*diffResultSD;
	float 				*sumResultSD;
	float 				*diffResultFD;
	float 				*sumResultFD;
	float 				*diffResultLR;
	float 				*sumResultHD;

	float				*magSenDefaultMaxSDValue;
	float				*magSenDefaultMaxFDValue;
	float				*magSenDefaultMaxHDValue;
	float				*magSenReferenceReading;

	// MyBattteryMeter

	float				*lastVoltage;

	// MyServo

	bool				*isServoStarted;
	int16_t				*lastTurningAngle;
	int32_t				*lastAngleListSum;

	MyConfig::
	SmartCarTurning		*TurningState;
	MyConfig::
	SmartCarPosition	*PositionState;

};
