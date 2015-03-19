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
	float			MyMotorSpeedControlRef = 300.0f;
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
	float			MyServoDefaultMaxSDValue = 0.0f;
	float			MyServoDefaultMinSDValue = 0.0f;
	float			MyServoDefaultMaxFDValue = 0.0f;
	float			MyServoDefaultMinFDValue = 0.0f;
	float			MyServoDefaultMaxLRValue = 0.0f;
	float			MyServoDefaultMinLRValue = 0.0f;

	// MyMotor
	uint8_t			MyMotorId = 0;
	int32_t			MyEncoderTargetCount = 1000;

	// MyEncoder
	uint8_t			MyEncoderId = 0;

	// MyBatteryMeter

	float			MyBatteryMeterVoltageRatio = 0.32984f;
	float			MyBatteryMeterUpdateEvery = 0.02f;
	float			MyBatteryMeterVoltageMax = 8.0f;
	float			MyBatteryMeterVoltageMin = 7.2f;

	uint16_t		MyBatteryMeterHighColor = 0x063F;
	uint16_t		MyBatteryMeterNormalColor = 0x07E0;
	uint16_t		MyBatteryMeterLowColor = 0xF800;

	// MyLcd
	uint16_t		MyLcdBackgroundColor = 0x0000;
	uint16_t		MyLcdTextColor = 0xFFFF;
	bool			MyLcdIsRevert = true;
	uint8_t			MyLcdFps = 60;

	// MySmartCar
	SmartCarPowerMode
					MySmartCarPowerMode = MyConfig::SmartCarPowerMode::kLowFrictionMode;
	SmartCarTurningMode
					MySmartCarTurningMode = MyConfig::SmartCarTurningMode::kStraightLine;

};
