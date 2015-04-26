/*
 * MyResource.cpp
 *
 *  Created on: Apr 25, 2015
 *      Author: Peter
 */

#include "MyResource.h"
#include "MySmartCar.h"

MyResource *m_resInstance = nullptr;

MyResource::MyResource(MySmartCar *sc)
{
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

// Servo
float MyResource::ConfigTable::ServoConfig::Reference = 0.0f;
float MyResource::ConfigTable::ServoConfig::Kp = 0.0f;
float MyResource::ConfigTable::ServoConfig::Ki = 0.0f;
float MyResource::ConfigTable::ServoConfig::Kd = 0.0f;

// Motor
float MyResource::ConfigTable::MotorConfig::Reference = 100.0f;
float MyResource::ConfigTable::MotorConfig::Kp = 0.0f;
float MyResource::ConfigTable::MotorConfig::Ki = 0.0f;
float MyResource::ConfigTable::MotorConfig::Kd = 0.0f;

// BatteryMeter
float MyResource::ConfigTable::BatteryMeterConfig::Ratio = 3.3f;
float MyResource::ConfigTable::BatteryMeterConfig::MaxVoltage = 8.2f;
float MyResource::ConfigTable::BatteryMeterConfig::MinVoltage = 7.6f;

// Lcd
uint8_t MyResource::ConfigTable::LcdConfig::Fps = 60;
uint16_t MyResource::ConfigTable::LcdConfig::BgColor = 0;
uint16_t MyResource::ConfigTable::LcdConfig::TxtColor = -1;
uint16_t MyResource::ConfigTable::LcdConfig::HighPowerColor = 0x063F;
uint16_t MyResource::ConfigTable::LcdConfig::NormalPowerColor = 0x07E0;
uint16_t MyResource::ConfigTable::LcdConfig::LowPowerColor = 0xF800;
