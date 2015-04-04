/*
 * MyLcd.h
 *
 *  Created on: Mar 16, 2015
 *      Author: Peter
 */

#pragma once

#include <libsc/battery_meter.h>
#include <libsc/lcd.h>
#include <libsc/lcd_console.h>
#include <libsc/simple_buzzer.h>
#include <libbase/k60/gpio.h>

#include "MyConfig.h"
#include "MyVar.h"
#include "MyLoop.h"

using namespace libsc;
using namespace libbase::k60;

class MyLcd
{

public:

	class MyBatteryMeter
	{

	public:

		MyBatteryMeter(MyConfig &config, MyVar &vars);

		float isUpdateVoltageNeeded(void);
		float getRealVoltage(void);

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

		explicit MyBuzzer(void);

		void setEnabled(bool enabled);

	private:

		Gpio					m_mybuzzer;

	};

	const static char endl = '\n';

	explicit MyLcd(MyConfig &config, MyVar &vars, MyLoop &loop);

	static void onDraw(void);

	static MyLcd *getMyLcdInstance(void);
	static LcdConsole *getLcdConsoleInstance(void);

	MyLcd &setRow(const uint8_t &row);
	MyLcd &operator<<(const char c);
	MyLcd &operator<<(const char *str);
	MyLcd &operator<<(const float &f);
	MyLcd &operator<<(const uint8_t &ub);
	MyLcd &operator<<(const uint16_t &us);
	MyLcd &operator<<(const uint32_t &ui);
	MyLcd &operator<<(const int8_t &b);
	MyLcd &operator<<(const int16_t &s);
	MyLcd &operator<<(const int32_t &i);

private:

	LcdConsole::Config getConsoleConfig(const MyConfig &config);
	LcdTypewriter::Config getWriterConfig(const MyConfig &config);

	St7735r						m_lcd;
	MyBuzzer					m_buzzer;
	LcdConsole					m_console;
	LcdTypewriter				m_writer;
	MyBatteryMeter				m_batteryMeter;

	uint16_t					m_backgroundColor;
	uint16_t					m_textColor;

	static const bool			BatteryOutlook[];
	static const bool			BatteryCharge[];

};
