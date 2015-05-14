/*
 * MyLcd.cpp
 *
 *  Created on: Apr 25, 2015
 *      Author: Peter
 */

#include <cmath>

#include <libbase/helper.h>
#include <libsc/st7735r.h>
#include <libsc/device_h/st7735r.h>
#include <libsc/lcd_console.h>
#include <libbase/misc_utils_c.h>
#include <libsc/lcd.h>
#include "MyLoop.h"
#include "MyLcd.h"
#include "MySmartCar.h"
#include "MyResource.h"

using namespace libsc;

const uint8_t MyLcd::BatteryOutlook[] =
{
	60, 255, 195, 195, 195, 195, 195,
	195, 195, 195, 195, 195, 255
};
const bool MyLcd::BatteryCharge[4 * 10] = { 1 };

LcdConsole::Config MyLcd::getLcdConsoleConfig(void)
{
	St7735r::Config config;
	config.is_revert = true;
	config.is_bgr = true;
	config.fps = MyResource::ConfigTable::LcdConfig::Fps;
	LcdConsole::Config consoleConfig;
	m_lcd = new St7735r(config);
	consoleConfig.bg_color = MyResource::ConfigTable::LcdConfig::BgColor;
	consoleConfig.text_color = MyResource::ConfigTable::LcdConfig::TxtColor;
	consoleConfig.lcd = m_lcd;
	return consoleConfig;
}

MyTypeWriter::Config MyLcd::getTypeWriterConfig(void)
{
	MyTypeWriter::Config config;
	config.bg_color = MyResource::ConfigTable::LcdConfig::BgColor;
	config.text_color = MyResource::ConfigTable::LcdConfig::TxtColor;
	config.is_clear_line = true;
	config.is_text_wrap = true;

	St7735r::Config lcdConfig;
	lcdConfig.is_revert = true;
	lcdConfig.is_bgr = true;
	lcdConfig.fps = MyResource::ConfigTable::LcdConfig::Fps;
	LcdConsole::Config consoleConfig;
	m_lcd = new St7735r(lcdConfig);

	config.lcd = m_lcd;
	return config;
}

MyLcd::MyBatteryMeter::MyBatteryMeter(void)
:
	BatteryMeter({ MyResource::ConfigTable::BatteryMeterConfig::Ratio })
{}

float MyLcd::MyBatteryMeter::getRealVoltage(void)
{
	return GetVoltage();
}

float MyLcd::MyBatteryMeter::getVolatagePercentage(void)
{
	return (1 - (GetVoltage() - MyResource::ConfigTable::BatteryMeterConfig::MinVoltage)
			/ (MyResource::ConfigTable::BatteryMeterConfig::MaxVoltage - MyResource::ConfigTable::BatteryMeterConfig::MinVoltage));
}

uint16_t MyLcd::MyBatteryMeter::getColor(float volt)
{
	if (volt < 0.25f)
		return MyResource::ConfigTable::LcdConfig::HighPowerColor;
	else if (volt < 0.75f)
		return MyResource::ConfigTable::LcdConfig::NormalPowerColor;
	else if (volt < 1.0f)
		return MyResource::ConfigTable::LcdConfig::LowPowerColor;
	return 0xFFFF;
}

MyLcd::MyLcd(void)
:
	MyTypeWriter(getTypeWriterConfig()),
	m_batteryMeter()
{}

void MyLcd::onDraw(const uint32_t &timeDelay)
{
//	float voltageRet = MyResource::smartCar().m_lcdConsole.m_batteryMeter.getVolatagePercentage();
//
//	MyResource::smartCar().m_lcdConsole.setRow(0) << "Voltage: " << MyResource::smartCar().m_lcdConsole.m_batteryMeter.getRealVoltage() << MyLcd::endl;
//	MyResource::smartCar().m_lcdConsole.m_lcd->SetRegion(St7735r::Rect(MyResource::smartCar().m_lcdConsole.m_lcd->GetW() - 10, 1, 8, 13));
//	MyResource::smartCar().m_lcdConsole.m_lcd->FillBits(
//			MyResource::ConfigTable::LcdConfig::TxtColor,
//			MyResource::ConfigTable::LcdConfig::BgColor,
//			MyResource::smartCar().m_lcdConsole.BatteryOutlook,
//			length(MyResource::smartCar().m_lcdConsole.BatteryOutlook) * 8);
//	MyResource::smartCar().m_lcdConsole.m_lcd->SetRegion(St7735r::Rect(MyResource::smartCar().m_lcdConsole.m_lcd->GetW() - 8, 3 + floor(10 * voltageRet), 4, 10 - floor(10 * voltageRet)));
//	MyResource::smartCar().m_lcdConsole.m_lcd->FillColor(MyResource::smartCar().m_lcdConsole.m_batteryMeter.getColor(voltageRet));
//	MyResource::smartCar().m_lcdConsole.m_lcd->ClearRegion();

	if (MyResource::smartCar().m_lcdConsole.m_batteryMeter.getVolatagePercentage() < 0.0f)
		MyResource::smartCar().m_led.SetEnable(true);
}

MyLcd &MyLcd::setRow(const uint8_t &row)
{
	SetCursorRow(row);
	return *this;
}

MyLcd &MyLcd::operator<<(const char c)
{
	WriteChar(c);
	return *this;
}

MyLcd &MyLcd::operator<<(const char *str)
{
	WriteString(str);
	return *this;
}

MyLcd &MyLcd::operator<<(const float &f)
{
	char buffer[20] = { 0 };
	size_t n = sprintf(buffer, "%.4f", f);
	WriteBuffer(buffer, n);
	return *this;
}

MyLcd &MyLcd::operator<<(const uint8_t &ub)
{
	char buffer[20] = { 0 };
	size_t n = sprintf(buffer, "%d", ub);
	WriteBuffer(buffer, n);
	return *this;
}

MyLcd &MyLcd::operator<<(const uint16_t &us)
{
	char buffer[20] = { 0 };
	size_t n = sprintf(buffer, "%d", us);
	WriteBuffer(buffer, n);
	return *this;
}

MyLcd &MyLcd::operator<<(const uint32_t &ui)
{
	char buffer[20] = { 0 };
	size_t n = sprintf(buffer, "%ld", ui);
	WriteBuffer(buffer, n);
	return *this;
}

MyLcd &MyLcd::operator<<(const int8_t &b)
{
	char buffer[20] = { 0 };
	size_t n = sprintf(buffer, "%d", b);
	WriteBuffer(buffer, n);
	return *this;
}

MyLcd &MyLcd::operator<<(const int16_t &s)
{
	char buffer[20] = { 0 };
	size_t n = sprintf(buffer, "%d", s);
	WriteBuffer(buffer, n);
	return *this;
}

MyLcd &MyLcd::operator<<(const int32_t &i)
{
	char buffer[20] = { 0 };
	size_t n = sprintf(buffer, "%ld", i);
	WriteBuffer(buffer, n);
	return *this;
}
