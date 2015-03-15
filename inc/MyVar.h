/*
 * MyVar.h
 *
 *  Created on: Mar 13, 2015
 *      Author: Peter
 */

#pragma once

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

	float				*MagSenLRLeft;
	float				*MagSenLRRight;

	float 				*diffResultSD;
	float 				*sumResultSD;

	float 				*diffResultFD;
	float 				*sumResultFD;

	float 				*diffResultLR;
	float 				*sumResultLR;

	// MyServo

	bool				*isServoStarted;
	int16_t				*lastTurningAngle;
	uint8_t				*lastTurningBeforeLowSignal;

};
