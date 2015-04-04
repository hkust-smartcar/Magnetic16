/*
 * MyLcd.h
 *
 *  Created on: Mar 17, 2015
 *      Author: Peter
 */

#include <cstdio>
#include <cmath>

#include <libsc/st7735r.h>
#include <libsc/lcd_console.h>
#include <libbase/k60/gpio.h>

#include "MyLcd.h"
#include "MyVar.h"
#include "MyConfig.h"
#include "MyLoop.h"
#include "MySmartCar.h"

using namespace libsc;
using namespace libbase::k60;

MyLcd *m_lcdInstance;

const bool MyLcd::BatteryOutlook[8 * 13] =
{
	false, false, true, true, true, true, false, false,
	true, true, true, true, true, true, true, true,
	true, true, false, false, false, false, true, true,
	true, true, false, false, false, false, true, true,
	true, true, false, false, false, false, true, true,
	true, true, false, false, false, false, true, true,
	true, true, false, false, false, false, true, true,
	true, true, false, false, false, false, true, true,
	true, true, false, false, false, false, true, true,
	true, true, false, false, false, false, true, true,
	true, true, false, false, false, false, true, true,
	true, true, false, false, false, false, true, true,
	true, true, true, true, true, true, true, true
};

const bool MyLcd::BatteryCharge[4 * 10] = { true };

St7735r::Config getLcdConfig(const MyConfig &myConfig)
{
	St7735r::Config config;
	config.is_revert = myConfig.MyLcdIsRevert;
	config.is_bgr = true;
	config.fps = myConfig.MyLcdFps;
	return config;
}

LcdConsole::Config MyLcd::getConsoleConfig(const MyConfig &config)
{
	LcdConsole::Config wconfig;
	wconfig.lcd = &m_lcd;
	wconfig.bg_color = config.MyLcdBackgroundColor;
	wconfig.text_color = config.MyLcdTextColor;
	return wconfig;
}

LcdTypewriter::Config MyLcd::getWriterConfig(const MyConfig &config)
{
	LcdTypewriter::Config wconfig;
	wconfig.lcd = &m_lcd;
	wconfig.bg_color = config.MyLcdBackgroundColor;
	wconfig.text_color = config.MyLcdTextColor;
	return wconfig;
}

Gpo::Config getBuzzerConfig(void)
{
	Gpo::Config config;
	config.pin = LIBSC_BUZZER0;
	config.is_high = false;
	return config;
}

MyLcd::MyBatteryMeter::MyBatteryMeter(MyConfig &config, MyVar &vars)
:
	m_lastVoltage(0.0f),
	m_updateThreshold(config.MyBatteryMeterUpdateEvery),
	m_batteryMeter({config.MyBatteryMeterVoltageRatio}),
	m_batteryMinVoltage(config.MyBatteryMeterVoltageMin),
	m_batteryMaxVoltage(config.MyBatteryMeterVoltageMax),
	m_batteryReferenceRange(m_batteryMaxVoltage - m_batteryMinVoltage),
	m_batteryLowColor(config.MyBatteryMeterLowColor),
	m_batteryNormalColor(config.MyBatteryMeterNormalColor),
	m_batteryHighColor(config.MyBatteryMeterHighColor)
{
	vars.lastVoltage = &m_lastVoltage;
}

float MyLcd::MyBatteryMeter::getVolatagePercentage(void)
{
	return (1 - (m_batteryMeter.GetVoltage() - m_batteryMinVoltage) / m_batteryReferenceRange);
}

float MyLcd::MyBatteryMeter::getRealVoltage(void)
{
	return m_batteryMeter.GetVoltage();
}

float MyLcd::MyBatteryMeter::isUpdateVoltageNeeded(void)
{
	float newVoltage = getVolatagePercentage();
	if (newVoltage - m_lastVoltage >= m_updateThreshold)
	{
		m_lastVoltage = newVoltage;
		return newVoltage;
	}
	else
		return 10.0f;
}

uint16_t MyLcd::MyBatteryMeter::getColor(void)
{
	if (m_lastVoltage < 0.25f)
		return m_batteryHighColor;
	else if (m_lastVoltage < 0.75f)
		return m_batteryNormalColor;
	else if (m_lastVoltage < 1.0f)
		return m_batteryLowColor;
	return 0xFFFF;
}

MyLcd::MyBuzzer::MyBuzzer(void)
:
		m_mybuzzer(getBuzzerConfig())
{}

void MyLcd::MyBuzzer::setEnabled(bool enabled)
{
	m_mybuzzer.Set(enabled);
}

MyLcd::MyLcd(MyConfig &config, MyVar &vars, MyLoop &loop)
:
	m_backgroundColor(config.MyLcdBackgroundColor),
	m_textColor(config.MyLcdTextColor),
	m_lcd(getLcdConfig(config)),
	m_console(getConsoleConfig(config)),
	m_batteryMeter(config, vars),
	m_writer(getWriterConfig(config))
{
	m_lcdInstance = this;
	m_lcd.Clear(config.MyLcdBackgroundColor);
	loop.addFunctionToLoop(&onDraw, LOOP_IMMEDIATELY, 1000);
}

void MyLcd::onDraw(void)
{
	char buffer[125] = { 0 };
	float voltageRet = m_lcdInstance->m_batteryMeter.isUpdateVoltageNeeded();

	if (voltageRet <= 1.0f)
	{
		m_lcdInstance->m_console.SetCursorRow(0);
		int n = sprintf(buffer, "Voltage: %.2fV\n", m_lcdInstance->m_batteryMeter.getRealVoltage());
		m_lcdInstance->m_console.WriteBuffer(buffer, n);
		m_lcdInstance->m_lcd.SetRegion(St7735r::Rect(m_lcdInstance->m_lcd.GetW() - 10, 1, 8, 13));
		m_lcdInstance->m_lcd.FillBits(m_lcdInstance->m_textColor, m_lcdInstance->m_backgroundColor, m_lcdInstance->BatteryOutlook, length(m_lcdInstance->BatteryOutlook));

		m_lcdInstance->m_lcd.SetRegion(St7735r::Rect(m_lcdInstance->m_lcd.GetW() - 8, 3 + floor(10 * voltageRet), 4, 10 - floor(10 * voltageRet)));
		m_lcdInstance->m_lcd.FillColor(m_lcdInstance->m_batteryMeter.getColor());
		m_lcdInstance->m_lcd.ClearRegion();
	}
}

MyLcd *MyLcd::getMyLcdInstance(void)
{
	return m_lcdInstance;
}

LcdConsole *MyLcd::getLcdConsoleInstance(void)
{
	return &m_lcdInstance->m_console;
}

MyLcd &MyLcd::setRow(const uint8_t &row)
{
	m_console.SetCursorRow(row);
	return *this;
}

MyLcd &MyLcd::operator<<(const char c)
{
	m_console.WriteChar(c);
	return * this;
}

MyLcd &MyLcd::operator<<(const char *str)
{
	m_console.WriteString(str);
	return *this;
}

MyLcd &MyLcd::operator<<(const float &f)
{
	char buffer[20] = { 0 };
	size_t n = sprintf(buffer, "%.4f", f);
	m_console.WriteBuffer(buffer, n);
	return *this;
}

MyLcd &MyLcd::operator<<(const uint8_t &ub)
{
	char buffer[20] = { 0 };
	size_t n = sprintf(buffer, "%d", ub);
	m_console.WriteBuffer(buffer, n);
	return *this;
}

MyLcd &MyLcd::operator<<(const uint16_t &us)
{
	char buffer[20] = { 0 };
	size_t n = sprintf(buffer, "%d", us);
	m_console.WriteBuffer(buffer, n);
	return *this;
}

MyLcd &MyLcd::operator<<(const uint32_t &ui)
{
	char buffer[20] = { 0 };
	size_t n = sprintf(buffer, "%d", ui);
	m_console.WriteBuffer(buffer, n);
	return *this;
}

MyLcd &MyLcd::operator<<(const int8_t &b)
{
	char buffer[20] = { 0 };
	size_t n = sprintf(buffer, "%d", b);
	m_console.WriteBuffer(buffer, n);
	return *this;
}

MyLcd &MyLcd::operator<<(const int16_t &s)
{
	char buffer[20] = { 0 };
	size_t n = sprintf(buffer, "%d", s);
	m_console.WriteBuffer(buffer, n);
	return *this;
}

MyLcd &MyLcd::operator<<(const int32_t &i)
{
	char buffer[20] = { 0 };
	size_t n = sprintf(buffer, "%d", i);
	m_console.WriteBuffer(buffer, n);
	return *this;
}
