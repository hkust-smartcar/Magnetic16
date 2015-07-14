/*
 * MyBatteryMeter.cpp
 *
 * Author: Peter
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
	if (!m_batteryMeterInstance)
		m_batteryMeterInstance = this;
}

float MyBatteryMeter::getBatteryVoltage(void)
{
	if (System::Time() - m_lastUpdateTime >= MyResource::ConfigTable::BatteryMeterConfig::UpdateFreq || System::Time() <= 5000)
		m_lastVoltage = GetVoltage();
	return m_lastVoltage;
}

float MyBatteryMeter::getBatteryPercentage(void)
{
	return ((getBatteryVoltage() - MyResource::ConfigTable::BatteryMeterConfig::MinVoltage) / (MyResource::ConfigTable::BatteryMeterConfig::MaxVoltage - MyResource::ConfigTable::BatteryMeterConfig::MinVoltage) * 100.0f);
}

//uint16_t MyBatteryMeter::getColor(void)
//{
//	float volt = getBatteryPercentage();
//	if (volt < 25.0f)
//		return MyResource::ConfigTable::LcdConfig::HighPowerColor;
//	else if (volt < 75.0f)
//		return MyResource::ConfigTable::LcdConfig::NormalPowerColor;
//	else if (volt < 100.0f)
//		return MyResource::ConfigTable::LcdConfig::LowPowerColor;
//	return 0xFFFF;
//}

uint8_t MyBatteryMeter::checkBattery(const uint32_t &)
{
	if (!MyResource::smartCar().m_motor.isEnabled())
	{
		bool isEnabled = MyResource::smartCar().m_buzzer.isEnabled();
		MyResource::smartCar().m_buzzer.setEnabled(true);

		float newPercentage = 0.0f, oldPercentage = m_batteryMeterInstance->getBatteryPercentage();

		while (oldPercentage != newPercentage)
		{
			DelayMsByTicks(500);
			oldPercentage = newPercentage;
			newPercentage = m_batteryMeterInstance->getBatteryPercentage();
		}

		if (newPercentage <= 0.0f)
			MyResource::smartCar().m_buzzer.set(true, false);
		else
		{
			int times = 0;
			if (newPercentage > 66.67f)
				times = 3;
			else if (newPercentage > 33.33f)
				times = 2;
			else
				times = 1;
			MyResource::smartCar().m_buzzer.beep(times, 100);
		}

		MyResource::smartCar().m_buzzer.setEnabled(isEnabled);
	}
}
