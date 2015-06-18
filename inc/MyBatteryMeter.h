/*
 * MyBatteryMeter.h
 *
 * Author: Peter
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <libsc/timer.h>
#include <libsc/battery_meter.h>
#include "MyResource.h"

using namespace libsc;

class MyBatteryMeter : BatteryMeter
{

public:

	MyBatteryMeter(void);

	float getBatteryVoltage(void);

	float getBatteryPercentage(void);

	uint16_t getColor(void);

	static uint8_t checkBattery(const uint32_t &timeDelay);

private:

	Timer::TimerInt	m_lastUpdateTime;
	float			m_lastVoltage;

};
