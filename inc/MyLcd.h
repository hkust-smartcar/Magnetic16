/*
 * MyLcd.h
 *
 *  Created on: Mar 16, 2015
 *      Author: Peter
 */

#pragma once

#include <libsc/k60/battery_meter.h>
#include <libsc/k60/lcd.h>
#include <libsc/k60/lcd_console.h>

#include "MyConfig.h"
#include "MyVar.h"
#include "MyLoop.h"

using namespace libsc::k60;

class MyLcd
{

public:

	class MyBatteryMeter
	{

	public:

		MyBatteryMeter(MyConfig &config, MyVar &vars);

		float isUpdateVoltageNeeded(void);

		uint16_t getColor(void);


	private:

		float getVolatagePercentage(void);

		BatteryMeter			m_batteryMeter;
		float					m_lastVoltage;
		float					m_updateThreshold;

		float					m_batteryMinVoltage;
		float					m_batteryMaxVoltage;
		float					m_batteryReferenceRange;

		float					m_batteryLowColor;
		float					m_batteryNormalColor;
		float					m_batteryHighColor;

	};

	class MyBuzzer
	{

	public:

		MyBuzzer(MyConfig &config, MyVar &vars);

	private:



	};

	explicit MyLcd(MyConfig &config, MyVar &vars, MyLoop &loop);

	static void onDraw(void);

private:

	LcdConsole::Config getWriterConfig(const MyConfig &config);

	St7735r						m_lcd;
	LcdConsole					m_console;
	MyBatteryMeter				m_batteryMeter;

	uint16_t					m_backgroundColor;
	uint16_t					m_textColor;

	static const bool			BatteryOutlook[];
	static const bool			BatteryCharge[];

};
