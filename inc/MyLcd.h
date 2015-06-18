/*
 * MyLcd.h
 *
 *  Created on: Apr 25, 2015
 *      Author: Peter
 */

#pragma once

#include <libbase/helper.h>
#include <libsc/battery_meter.h>
#include <libsc/lcd.h>
#include <libsc/lcd_console.h>
#include "MyResource.h"
#include "MyTypeWriter.h"

#define MAX_ASCII_INDEX 126
#define MIN_ASCII_INDEX 32

#define inRange(n, v, x) ((v < n)? n : ((v > x)? x : v))
#define length(v) (sizeof(v) / sizeof(v[0]))

using namespace libsc;
using namespace libbase::k60;

class MyLcd : private MyTypeWriter
{

public:

	const static char endl = '\n';

	explicit MyLcd(void);

	static void onDraw(const uint32_t &timeDelay);

	void setEnabled(const bool enabled);
	bool isEnabled(void);

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

	St7735r					*m_lcd;

private:

	Timer::TimerInt			m_lastUpdateTime;

	LcdConsole::Config getLcdConsoleConfig(void);
	MyTypeWriter::Config getTypeWriterConfig(void);

	static const bool BatteryCharge[];
	static const uint8_t BatteryOutlook[];

	bool					m_enabled;

	static MyLcd			*m_instance;

};
