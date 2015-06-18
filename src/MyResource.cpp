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


// Buzzer
int MyResource::ConfigTable::BuzzerConfig::WarningPitch = 70;
int MyResource::ConfigTable::BuzzerConfig::WarningTimes = 3;

// MagSen
float MyResource::ConfigTable::MagSenConfig::Kq = 0.6;
float MyResource::ConfigTable::MagSenConfig::Kr = 0.5;

// Servo
float MyResource::ConfigTable::ServoConfig::Reference = 0.0f;

float MyResource::ConfigTable::ServoConfig::TurningKpA = 1000.0f;
float MyResource::ConfigTable::ServoConfig::TurningKpB = 0.0f;
float MyResource::ConfigTable::ServoConfig::TurningKi = 0.0f;
float MyResource::ConfigTable::ServoConfig::TurningKd = 70.0f;

float MyResource::ConfigTable::ServoConfig::NormalKp = 400.0f;
float MyResource::ConfigTable::ServoConfig::NormalKi = 0.0f;
float MyResource::ConfigTable::ServoConfig::NormalKd = 60.0f;

float MyResource::ConfigTable::ServoConfig::WeightSD = 0.3589743f;
float MyResource::ConfigTable::ServoConfig::WeightFD = 0.1794872f;
float MyResource::ConfigTable::ServoConfig::WeightHD = 0.4615385f;

float MyResource::ConfigTable::ServoConfig::TurningThresholdSdValue = 0.7f;
float MyResource::ConfigTable::ServoConfig::ForceTurningThresholdStartSdOutput = 0.5f;
float MyResource::ConfigTable::ServoConfig::ForceTurningThresholdStartFdOutput = 0.8f;
float MyResource::ConfigTable::ServoConfig::ForceTurningThresholdStopFdOutput = 0.6f;
float MyResource::ConfigTable::ServoConfig::ForceTurningThresholdStopHdOutput = 0.1f;

uint32_t MyResource::ConfigTable::ServoConfig::UpdateFreq = 7;

// Motor
float MyResource::ConfigTable::MotorConfig::Reference = 800.0f;
//float MyResource::ConfigTable::MotorConfig::Kp = 0.072f;
float MyResource::ConfigTable::MotorConfig::Kp = 0.03f;
float MyResource::ConfigTable::MotorConfig::Ki = 0.0f;
float MyResource::ConfigTable::MotorConfig::Kd = 0.001f;

uint32_t MyResource::ConfigTable::MotorConfig::UpdateFreq = 20;

// VarMng
uint16_t MyResource::ConfigTable::VarMngConfig::UpdateFreq = 5;

// BatteryMeter
float MyResource::ConfigTable::BatteryMeterConfig::Ratio = 0.32591351983096153012896110238026f;
//float MyResource::ConfigTable::BatteryMeterConfig::Ratio = 0.3333333f;
float MyResource::ConfigTable::BatteryMeterConfig::MaxVoltage = 8.2f;
float MyResource::ConfigTable::BatteryMeterConfig::MinVoltage = 7.55f;
uint16_t MyResource::ConfigTable::BatteryMeterConfig::UpdateFreq = 30000;

// Lcd
uint8_t MyResource::ConfigTable::LcdConfig::Fps = 60;
uint16_t MyResource::ConfigTable::LcdConfig::BgColor = 0;
uint16_t MyResource::ConfigTable::LcdConfig::TxtColor = -1;
uint16_t MyResource::ConfigTable::LcdConfig::HighPowerColor = 0x063F;
uint16_t MyResource::ConfigTable::LcdConfig::NormalPowerColor = 0x07E0;
uint16_t MyResource::ConfigTable::LcdConfig::LowPowerColor = 0xF800;
uint16_t MyResource::ConfigTable::LcdConfig::UpdateFreq = 80;
