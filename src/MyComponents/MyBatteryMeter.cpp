/*
 * MyBatteryMeter.cpp
 *
 * Author: gowen
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include <libsc/timer.h>
#include <libsc/battery_meter.h>
#include "MyBatteryMeter.h"
#include "MySmartCar.h"
#include "MyResource.h"

using namespace libsc;

MyBatteryMeter *m_batteryMeterInstance = nullptr;

MyBatteryMeter::MyBatteryMeter(void)
:
	BatteryMeter({ MyResource::ConfigTable::BatteryMeterConfig::Ratio }),
	m_lastUpdateTime(0),
	m_lastVoltage(GetVoltage())
{
	System::Init();
	m_lastUpdateTime = System::Time();
	if (!m_batteryMeterInstance)
		m_batteryMeterInstance = this;
}

float MyBatteryMeter::getBatteryVoltage(void)
{
	if (System::Time() - m_lastUpdateTime >= MyResource::ConfigTable::BatteryMeterConfig::UpdateFreq)
		m_lastVoltage = GetVoltage();
	return m_lastVoltage;
}

float MyBatteryMeter::getBatteryPercentage(void)
{
	return ((getBatteryVoltage() - MyResource::ConfigTable::BatteryMeterConfig::MinVoltage) / (MyResource::ConfigTable::BatteryMeterConfig::MaxVoltage - MyResource::ConfigTable::BatteryMeterConfig::MinVoltage) * 100.0f);
}

uint16_t MyBatteryMeter::getColor(void)
{
	float volt = getBatteryPercentage();
	if (volt < 25.0f)
		return MyResource::ConfigTable::LcdConfig::HighPowerColor;
	else if (volt < 75.0f)
		return MyResource::ConfigTable::LcdConfig::NormalPowerColor;
	else if (volt < 100.0f)
		return MyResource::ConfigTable::LcdConfig::LowPowerColor;
	return 0xFFFF;
}

void MyBatteryMeter::checkBattery(const uint32_t &timeDelay)
{
	if (m_batteryMeterInstance->getBatteryPercentage() < 1.0f && !MyResource::smartCar().m_motor.isEnabled())
		MyResource::smartCar().m_buzzer.setEnabled(true);
}
