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

MyLcd *m_lcdInstance = nullptr;

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

MyLcd::MyLcd(void)
:
	MyTypeWriter(getTypeWriterConfig()),
	m_lastUpdateTime(0)
{
	if (!m_lcdInstance)
		m_lcdInstance = this;
}

void MyLcd::onDraw(const uint32_t &timeDelay)
{
	if (System::Time() - m_lcdInstance->m_lastUpdateTime > MyResource::ConfigTable::LcdConfig::UpdateFreq)
		MyResource::smartCar().m_lcdConsole.setRow(0) << "Motor_Ref:" << MyResource::ConfigTable::MotorConfig::Reference << MyLcd::endl
													  << "       Kp:" << MyResource::ConfigTable::MotorConfig::Kp << MyLcd::endl
													  << "       Ki:" << MyResource::ConfigTable::MotorConfig::Ki << MyLcd::endl
													  << "       Kd:" << MyResource::ConfigTable::MotorConfig::Kd << MyLcd::endl << MyLcd::endl
													  << "Servo_Ref:" << MyResource::ConfigTable::ServoConfig::Reference << MyLcd::endl
													  << "       Kp:" << MyResource::ConfigTable::ServoConfig::Kp << MyLcd::endl
													  << "       Ki:" << MyResource::ConfigTable::ServoConfig::Ki << MyLcd::endl
													  << "       Kd:" << MyResource::ConfigTable::ServoConfig::Kd << MyLcd::endl;
//	{
//		array<float, 2> valueSD = MyResource::smartCar().m_servo.m_MagSen[MyMagSen::SD].getFilteredValue();
//		array<float, 2> valueFD = MyResource::smartCar().m_servo.m_MagSen[MyMagSen::FD].getFilteredValue();
//		array<float, 2> valueHD = MyResource::smartCar().m_servo.m_MagSen[MyMagSen::HD].getFilteredValue();
//		MyResource::smartCar().m_lcdConsole.setRow(0) << "SD:" << MyLcd::endl
//													  << valueSD[MyMagSen::LEFT] << " " << valueSD[MyMagSen::RIGHT] << MyLcd::endl << MyLcd::endl
//													  << "FD:" << MyLcd::endl
//													  << valueFD[MyMagSen::LEFT] << " " << valueFD[MyMagSen::RIGHT] << MyLcd::endl << MyLcd::endl
//													  << "HD:" << MyLcd::endl
//													  << valueHD[MyMagSen::LEFT] << " " << valueHD[MyMagSen::RIGHT] << MyLcd::endl << MyLcd::endl
//													  << "FnlVal: " << MyResource::smartCar().m_servo.m_lastAngle << MyLcd::endl
//													  << "Angle: " << MyResource::smartCar().m_servo.getFinalAngle() << "  ";
//	}
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
