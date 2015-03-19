/*
 * MyLcd.h
 *
 *  Created on: Mar 17, 2015
 *      Author: Peter
 */

#include <cstdio>
#include <cmath>

#include <libsc/k60/st7735r.h>
#include <libsc/k60/lcd_console.h>

#include "MyLcd.h"
#include "MyVar.h"
#include "MyConfig.h"
#include "MyLoop.h"
#include "MySmartCar.h"

using namespace libsc::k60;

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

LcdConsole::Config MyLcd::getWriterConfig(const MyConfig &config)
{
	LcdConsole::Config wconfig;
	wconfig.lcd = &m_lcd;
	wconfig.bg_color = config.MyLcdBackgroundColor;
	wconfig.text_color = config.MyLcdTextColor;
	return wconfig;
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

float MyLcd::MyBatteryMeter::isUpdateVoltageNeeded(void)
{
	float newVoltage = getVolatagePercentage();
	if (newVoltage - m_lastVoltage >= m_updateThreshold)
	{
		m_lastVoltage = newVoltage;
		return newVoltage;
	}
	else
		return 0.0f;
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

MyLcd::MyLcd(MyConfig &config, MyVar &vars, MyLoop &loop)
:
	m_backgroundColor(config.MyLcdBackgroundColor),
	m_textColor(config.MyLcdTextColor),
	m_lcd(getLcdConfig(config)),
	m_console(getWriterConfig(config)),
	m_batteryMeter(config, vars)
{
	m_lcdInstance = this;
	m_lcd.Clear(config.MyLcdBackgroundColor);
	loop.addFunctionToLoop(&onDraw, LOOP_IMMEDIATELY, 500);
}

void MyLcd::onDraw(void)
{
	char buffer[125] = { 0 };
	float voltageRet = m_lcdInstance->m_batteryMeter.isUpdateVoltageNeeded();

	if (voltageRet)
	{
		m_lcdInstance->m_lcd.SetRegion(St7735r::Rect(m_lcdInstance->m_lcd.GetW() - 10, 2, 8, 13));
		m_lcdInstance->m_lcd.FillBits(m_lcdInstance->m_textColor, m_lcdInstance->m_backgroundColor, m_lcdInstance->BatteryOutlook, length(m_lcdInstance->BatteryOutlook));

		m_lcdInstance->m_lcd.SetRegion(St7735r::Rect(m_lcdInstance->m_lcd.GetW() - 8, 4 + round(10 * voltageRet), 4, 10 - round(10 * voltageRet)));
		m_lcdInstance->m_lcd.FillColor(m_lcdInstance->m_batteryMeter.getColor());
		m_lcdInstance->m_lcd.ClearRegion();
	}
}
