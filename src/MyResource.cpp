/*
 * MyResource.cpp
 *
 * Author: Peter
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include <libsc/system.h>
#include "MyResource.h"
#include "MySmartCar.h"

MyResource *m_resInstance = nullptr;

MyResource::MyResource(MySmartCar *sc)
{
	System::Init();
	if (!m_resInstance && sc)
	{
		m_resInstance = this;
		m_smartCar = sc;
	}
}

MySmartCar &MyResource::smartCar(void)
{
	return *m_resInstance->m_smartCar;
}


#ifdef IS_FRONT

#if IS_FRONT == 1

	// Buzzer
//	uint8_t MyResource::ConfigTable::BuzzerConfig::WarningPitch = 70;
//	uint8_t MyResource::ConfigTable::BuzzerConfig::WarningTimes = 3;

	// MagSen
	float MyResource::ConfigTable::MagSenConfig::Kq = 0.6;
	float MyResource::ConfigTable::MagSenConfig::Kr = 0.5;

	// Servo
//	float MyResource::ConfigTable::ServoConfig::Reference = 0.0f;


	float MyResource::ConfigTable::ServoConfig::NormalKp = 174.0f;
//	float MyResource::ConfigTable::ServoConfig::NormalKi = 0.0f;
	float MyResource::ConfigTable::ServoConfig::NormalKd = 9.0f;

//	float MyResource::ConfigTable::ServoConfig::NormalWeightSD = 0.3589743f;
//	float MyResource::ConfigTable::ServoConfig::NormalWeightFD = 0.1794872f;
//	float MyResource::ConfigTable::ServoConfig::NormalWeightHD = 0.4615385f;


	float MyResource::ConfigTable::ServoConfig::TurningKpA =  350.0f; //250.0f;
	float MyResource::ConfigTable::ServoConfig::TurningKpB = 540.0f; //120.0f;
//	float MyResource::ConfigTable::ServoConfig::TurningKi = 0.0f;
	float MyResource::ConfigTable::ServoConfig::TurningKd = 15.0f;

//	float MyResource::ConfigTable::ServoConfig::TurningWeightSD = 0.1f;
//	float MyResource::ConfigTable::ServoConfig::TurningWeightFD = 0.325f;
//	float MyResource::ConfigTable::ServoConfig::TurningWeightHD = 0.525f;

	float MyResource::ConfigTable::ServoConfig::CrossRoadThresholdFdValue = 2.0f;
	float MyResource::ConfigTable::ServoConfig::TurningThresholdHdValue = 0.3f;
	float MyResource::ConfigTable::ServoConfig::SdNoSignalThreshold = 0.675f;
	float MyResource::ConfigTable::ServoConfig::Turning90DegreeThresholdSdOutput = 0.7f;
	float MyResource::ConfigTable::ServoConfig::Turning90DegreeThresholdFdAvg = 1.0f;


	uint32_t MyResource::ConfigTable::ServoConfig::UpdateFreq = 4;

	// Motor
	float MyResource::ConfigTable::MotorConfig::Reference = 1500.0f;
	float MyResource::ConfigTable::MotorConfig::Kp = 0.025f;
//	float MyResource::ConfigTable::MotorConfig::Ki = 0.0f;
	float MyResource::ConfigTable::MotorConfig::Kd = 0.018f;

//	float MyResource::ConfigTable::MotorConfig::TurningSpeedRatio = 0.8f;
//	float MyResource::ConfigTable::MotorConfig::Turning90DegreeSpeedRatio = 0.65f;
//	float MyResource::ConfigTable::MotorConfig::MaxSpeedRatio = 1.3f;
//
//	uint32_t MyResource::ConfigTable::MotorConfig::TimeForReachingMaxSpeed = 3000;
//	int16_t MyResource::ConfigTable::MotorConfig::EmergencyStopThreshold = 0;

	uint32_t MyResource::ConfigTable::MotorConfig::UpdateFreq = 20;

	// VarMng
	uint16_t MyResource::ConfigTable::VarMngConfig::UpdateFreq = 20;

	// BatteryMeter
	float MyResource::ConfigTable::BatteryMeterConfig::Ratio = 0.32591351983096153012896110238026f;
	float MyResource::ConfigTable::BatteryMeterConfig::MaxVoltage = 8.2f;
	float MyResource::ConfigTable::BatteryMeterConfig::MinVoltage = 7.6f;
	uint16_t MyResource::ConfigTable::BatteryMeterConfig::UpdateFreq = 30000;

	// Lcd
	uint8_t MyResource::ConfigTable::LcdConfig::Fps = 60;
	uint16_t MyResource::ConfigTable::LcdConfig::LcdConfig::BgColor = 0;
	uint16_t MyResource::ConfigTable::LcdConfig::TxtColor = -1;
//	uint16_t MyResource::ConfigTable::LcdConfig::HighPowerColor = 0x063F;
//	uint16_t MyResource::ConfigTable::LcdConfig::NormalPowerColor = 0x07E0;
//	uint16_t MyResource::ConfigTable::LcdConfig::LowPowerColor = 0xF800;
	uint16_t MyResource::ConfigTable::LcdConfig::UpdateFreq = 80;

#else

	// Buzzer
	int MyResource::ConfigTable::BuzzerConfig::WarningPitch = 70;
	int MyResource::ConfigTable::BuzzerConfig::WarningTimes = 3;

	// MagSen
	float MyResource::ConfigTable::MagSenConfig::Kq = 0.6;
	float MyResource::ConfigTable::MagSenConfig::Kr = 0.5;

	// Servo
	float MyResource::ConfigTable::ServoConfig::Reference = 0.0f;


	float MyResource::ConfigTable::ServoConfig::NormalKp = 300.0f;
	float MyResource::ConfigTable::ServoConfig::NormalKi = 0.0f;
	float MyResource::ConfigTable::ServoConfig::NormalKd = 10.0f;

	float MyResource::ConfigTable::ServoConfig::NormalWeightSD = 0.3589743f;
	float MyResource::ConfigTable::ServoConfig::NormalWeightFD = 0.1794872f;
	float MyResource::ConfigTable::ServoConfig::NormalWeightHD = 0.4615385f;


	float MyResource::ConfigTable::ServoConfig::TurningKpA = 2600.0f;
	float MyResource::ConfigTable::ServoConfig::TurningKpB = 0.0f;
	float MyResource::ConfigTable::ServoConfig::TurningKi = 0.0f;
	float MyResource::ConfigTable::ServoConfig::TurningKd = 40.0f;

	float MyResource::ConfigTable::ServoConfig::TurningWeightSD = 0.1f;
	float MyResource::ConfigTable::ServoConfig::TurningWeightFD = 0.325f;
	float MyResource::ConfigTable::ServoConfig::TurningWeightHD = 0.525f;

	float MyResource::ConfigTable::ServoConfig::TurningThresholdSdValue = 0.8f;
	float MyResource::ConfigTable::ServoConfig::ForceTurningThresholdStartSdOutput = 0.5f;
	float MyResource::ConfigTable::ServoConfig::ForceTurningThresholdStartFdOutput = 0.8f;
	float MyResource::ConfigTable::ServoConfig::ForceTurningThresholdStopFdOutput = 0.6f;
	float MyResource::ConfigTable::ServoConfig::ForceTurningThresholdStopHdOutput = 0.1f;


	uint32_t MyResource::ConfigTable::ServoConfig::UpdateFreq = 7;

	// Motor
	float MyResource::ConfigTable::MotorConfig::Reference = 1200.0f;
	float MyResource::ConfigTable::MotorConfig::Kp = 0.025f;
	float MyResource::ConfigTable::MotorConfig::Ki = 0.0f;
	float MyResource::ConfigTable::MotorConfig::Kd = 0.018f;

	float MyResource::ConfigTable::MotorConfig::TurningSpeedRatio = 0.45f;
	float MyResource::ConfigTable::MotorConfig::Turning90DegreeSpeedRatio = 0.3f;
	float MyResource::ConfigTable::MotorConfig::MaxSpeedRatio = 1.8f;

	uint32_t MyResource::ConfigTable::MotorConfig::TimeForReachingMaxSpeed = 2000;
	int16_t MyResource::ConfigTable::MotorConfig::EmergencyStopThreshold = 0;

	uint32_t MyResource::ConfigTable::MotorConfig::UpdateFreq = 20;

	// VarMng
	uint16_t MyResource::ConfigTable::VarMngConfig::UpdateFreq = 20;

	// BatteryMeter
	float MyResource::ConfigTable::BatteryMeterConfig::Ratio = 0.32591351983096153012896110238026f;
	float MyResource::ConfigTable::BatteryMeterConfig::MaxVoltage = 8.2f;
	float MyResource::ConfigTable::BatteryMeterConfig::MinVoltage = 7.6f;
	uint16_t MyResource::ConfigTable::BatteryMeterConfig::UpdateFreq = 30000;

	// Lcd
	uint8_t MyResource::ConfigTable::LcdConfig::Fps = 60;
	uint16_t MyResource::ConfigTable::LcdConfig::LcdConfig::BgColor = 0;
	uint16_t MyResource::ConfigTable::LcdConfig::TxtColor = -1;
	uint16_t MyResource::ConfigTable::LcdConfig::HighPowerColor = 0x063F;
	uint16_t MyResource::ConfigTable::LcdConfig::NormalPowerColor = 0x07E0;
	uint16_t MyResource::ConfigTable::LcdConfig::LowPowerColor = 0xF800;
	uint16_t MyResource::ConfigTable::LcdConfig::UpdateFreq = 80;

#endif

#endif //IS_FRONT
