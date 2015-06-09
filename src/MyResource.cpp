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
float MyResource::ConfigTable::ServoConfig::Kp = 260.0f; // 300.0f
float MyResource::ConfigTable::ServoConfig::Ki = 0.0f;
float MyResource::ConfigTable::ServoConfig::Kd = 30.0f; // 10.0f

uint32_t MyResource::ConfigTable::ServoConfig::UpdateFreq = 10;

float MyResource::ConfigTable::ServoConfig::WeightSD = 0.7f;
float MyResource::ConfigTable::ServoConfig::WeightFD = 0.35f;
float MyResource::ConfigTable::ServoConfig::WeightHD = 0.9f;

// Motor
float MyResource::ConfigTable::MotorConfig::Reference = 1200.0f;
//float MyResource::ConfigTable::MotorConfig::Kp = 0.072f;
float MyResource::ConfigTable::MotorConfig::Kp = 0.03f;
float MyResource::ConfigTable::MotorConfig::Ki = 0.0f;
float MyResource::ConfigTable::MotorConfig::Kd = 0.001f;

uint32_t MyResource::ConfigTable::MotorConfig::UpdateFreq = 20;

// VarMng
uint16_t MyResource::ConfigTable::VarMngConfig::UpdateFreq = 10;

// BatteryMeter
float MyResource::ConfigTable::BatteryMeterConfig::Ratio = 0.32984f;
//float MyResource::ConfigTable::BatteryMeterConfig::Ratio = 0.3333333f;
float MyResource::ConfigTable::BatteryMeterConfig::MaxVoltage = 8.2f;
float MyResource::ConfigTable::BatteryMeterConfig::MinVoltage = 7.6f;
uint16_t MyResource::ConfigTable::BatteryMeterConfig::UpdateFreq = 30000;

// Lcd
uint8_t MyResource::ConfigTable::LcdConfig::Fps = 60;
uint16_t MyResource::ConfigTable::LcdConfig::BgColor = 0;
uint16_t MyResource::ConfigTable::LcdConfig::TxtColor = -1;
uint16_t MyResource::ConfigTable::LcdConfig::HighPowerColor = 0x063F;
uint16_t MyResource::ConfigTable::LcdConfig::NormalPowerColor = 0x07E0;
uint16_t MyResource::ConfigTable::LcdConfig::LowPowerColor = 0xF800;
uint16_t MyResource::ConfigTable::LcdConfig::UpdateFreq = 300;
