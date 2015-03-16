/*
 * MyConfig.h
 *
 *  Created on: Mar 10, 2015
 *      Author: Peter
 */

#pragma once

class MyConfig
{

public:

	enum struct SmartCarPowerMode
	{
		kNormalMode = 0,
		kLowFrictionMode,
		kRcCarMode
	};

	enum struct SmartCarTurningMode
	{
		kStraightLine = 0,
		kNormal,
		k90Degree,
		kCrossRoad
	};

	// Put All Configuration Here

	// MyMotorPID
	float			MyMotorSpeedControlRef = 100.0f;
	float			MyMotorSpeedControlKp = 0.005f;
	float			MyMotorSpeedControlKi = 0.0f;
	float			MyMotorSpeedControlKd = 0.000f;

	// MyServoTurningPID
	float			MyServoTurningRef = 0.0f;
	float			MyServoTurningKp = 1.0f;
	float			MyServoTurningKi = 0.0f;
	float			MyServoTurningKd = 0.0f;

	// MyMagSen
	uint8_t			MyMagSenPairCount = 3;
	uint8_t			MyMagSenPairId0 = 0;
	uint8_t			MyMagSenPairId1 = 2;
	uint8_t			MyMagSenPairId2 = 4;

	float			MyMagSenFilterQ = 0.0015f;
	float			MyMagSenFilterR = 0.9f;

	float			MyMagSenDistanceWhenMaxDiff = 10.0f;
	float			MyMagMaxSenReadingDiff = 1.4f;

	// MyServo
	uint8_t			MyServoId = 0;
	float			MyServoFactorSD = 1.0f;
	float			MyServoFactorFD = 0.0f;
	float			MyServoFactorLR = 0.0f;

	// MyMotor
	uint8_t			MyMotorId = 0;
	int32_t			MyEncoderTargetCount = 1000;

	// MyEncoder
	uint8_t			MyEncoderId = 0;

	// MySmartCar
	SmartCarPowerMode
					MySmartCarPowerMode = MyConfig::SmartCarPowerMode::kLowFrictionMode;
	SmartCarTurningMode
					MySmartCarTurningMode = MyConfig::SmartCarTurningMode::kStraightLine;

};
