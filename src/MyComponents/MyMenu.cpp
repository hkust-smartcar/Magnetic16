/*
 * MyMenu.cpp
 *
 * Author: Peter
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include <libbase/k60/gpio.h>
#include <libsc/button.h>
#include <libsc/st7735r.h>
#include "MyServo.h"
#include "MyMenu.h"
#include "MyResource.h"
#include "MySmartCar.h"

using namespace libsc;
using namespace libbase::k60;

MyMenu *MyMenu::m_instance = nullptr;

Gpi::Config MyMenu::getSwitchConfig(const uint8_t id)
{
	Gpi::Config config;
	switch (id)
	{
	case 0:
		config.pin = LIBSC_SWITCH0; break;

	case 1:
		config.pin = LIBSC_SWITCH1; break;

	case 2:
		config.pin = LIBSC_SWITCH2; break;

	case 3:
		config.pin = LIBSC_SWITCH3; break;

	case 4:
		config.pin = LIBSC_SWITCH4; break;

	case 5:
		config.pin = LIBSC_SWITCH5; break;

	case 6:
		config.pin = LIBSC_SWITCH6; break; // NewCar: gged

	case 7:
		config.pin = LIBSC_SWITCH7; break;
	}
	config.interrupt = Pin::Config::Interrupt::kBoth;
	config.isr = &switchOnCheckedChanged;
	return config;
}

Joystick::Config MyMenu::getJoystickConfig(void)
{
	Joystick::Config config;
	config.id = 0;
	for (uint8_t i = 0; i < 5; i++)
	{
		config.listeners[i] = &joystickOnChanged;
		config.listener_triggers[i] = Joystick::Config::Trigger::kDown;
	}
	config.is_active_low = true;
	return config;
}

Button::Config MyMenu::getButtonConfig(const uint8_t id)
{
	Button::Config config;
	config.id = id;
	config.is_active_low = true;
	config.listener = &buttonOnPress;
	config.listener_trigger = Button::Config::Trigger::kDown;
	return config;
}

MyMenu::MyMenu(MyLcd &lcd)
:
	m_lcd(lcd),
	m_button({ Button(getButtonConfig(0)),
			   Button(getButtonConfig(1)) }),
	m_switch({ Switch(getSwitchConfig(7)),
			   Switch(getSwitchConfig(6)),
			   Switch(getSwitchConfig(5)),
			   Switch(getSwitchConfig(4)),
			   Switch(getSwitchConfig(3)),
			   Switch(getSwitchConfig(2)),
			   Switch(getSwitchConfig(1)),
			   Switch(getSwitchConfig(0)) }),
	m_joystick(getJoystickConfig()),
	m_isChangingCol(false),
	m_curRowsIndex(0),
	m_curColsIndex(0),
	m_curPagesIndex(Pages::Static)
{
	System::Init();
	if (!m_instance)
		m_instance = this;
}

void MyMenu::onDraw(const uint32_t &timeDelay)
{
	if (MyResource::smartCar().m_lcdConsole.isEnabled() && m_instance->m_curPagesIndex != Pages::Static)
	{
		switch (m_instance->m_curPagesIndex)
		{
		default:
			assert(false);
			break;

		case Pages::Status:
			MyResource::smartCar().m_lcdConsole.setRow(0) << " Status: " << MyLcd::endl << MyLcd::endl

														  << "  Car State: " << ((MyResource::smartCar().m_servo.m_isPidNonLinear)? "Turning" : "GoStaright") << MyLcd::endl
														  << "  Servo Angle: " << (int16_t)(MyResource::smartCar().m_servo.m_lastDegree - MID_SERVO_DEGREE) << MyLcd::endl
														  << "  Motor Speed: " << (MyResource::smartCar().m_motor.getSpeed()) << MyLcd::endl;
			break;

		case MyMenu::Pages::MagSens:
			array<float, 2> valueSD, valueFD, valueHD;
			switch (m_instance->m_curColsIndex)
			{
			default:
				assert(false);
				break;

			case 0:
				valueSD = MyResource::smartCar().m_servo.m_MagSen[MyMagSen::SD].getRawValue();
				valueFD = MyResource::smartCar().m_servo.m_MagSen[MyMagSen::FD].getRawValue();
				valueHD = MyResource::smartCar().m_servo.m_MagSen[MyMagSen::HD].getRawValue();
				break;

			case 1:
				valueSD = MyResource::smartCar().m_servo.m_MagSen[MyMagSen::SD].getStandardizedValue();
				valueFD = MyResource::smartCar().m_servo.m_MagSen[MyMagSen::FD].getStandardizedValue();
				valueHD = MyResource::smartCar().m_servo.m_MagSen[MyMagSen::HD].getStandardizedValue();
				break;
			}
			MyResource::smartCar().m_lcdConsole.setRow(0) << " MagSens: " << MyLcd::endl << MyLcd::endl

														  << "  SdValue:" << valueSD[MyMagSen::LEFT] << " " << valueSD[MyMagSen::RIGHT] << MyLcd::endl
														  << "  FdValue:" << valueFD[MyMagSen::LEFT] << " " << valueFD[MyMagSen::RIGHT] << MyLcd::endl
														  << "  HdValue:" << valueHD[MyMagSen::LEFT] << " " << valueHD[MyMagSen::RIGHT] << MyLcd::endl

														  << "  SdOutput:" << MyResource::smartCar().m_servo.m_MagSen[MyMagSen::SD].getOutputValue() << MyLcd::endl
														  << "  FdOutput:" << MyResource::smartCar().m_servo.m_MagSen[MyMagSen::FD].getOutputValue() << MyLcd::endl
														  << "  HdOutput:" << MyResource::smartCar().m_servo.m_MagSen[MyMagSen::HD].getOutputValue() << MyLcd::endl

														  << "  SdAvg: " << MyResource::smartCar().m_servo.m_MagSen[MyMagSen::SD].getFilteredValueAvg() << MyLcd::endl
														  << "  FdAvg: " << MyResource::smartCar().m_servo.m_MagSen[MyMagSen::FD].getFilteredValueAvg() << MyLcd::endl
														  << "  HdAvg: " << MyResource::smartCar().m_servo.m_MagSen[MyMagSen::HD].getFilteredValueAvg() << MyLcd::endl;
			break;

		case MyMenu::Pages::Servo:
			MyResource::smartCar().m_lcdConsole << " Servo: " << MyLcd::endl << MyLcd::endl

												<< "  Output: "<< MyResource::smartCar().m_servo.m_lastError << MyLcd::endl
											    << "  Angle: " << (int16_t)(MyResource::smartCar().m_servo.m_lastDegree - MID_SERVO_DEGREE) << "   " << MyLcd::endl
											    << "  NonLinear: " << (uint8_t)MyResource::smartCar().m_servo.m_isPidNonLinear << MyLcd::endl << MyLcd::endl

												<< "  SR: "<< MyResource::smartCar().m_servo.m_MagSen[MyMagSen::SD].m_ratio[MyMagSen::LEFT] << " " << MyResource::smartCar().m_servo.m_MagSen[MyMagSen::SD].m_ratio[MyMagSen::RIGHT] << MyLcd::endl
												<< "  FR: "<< MyResource::smartCar().m_servo.m_MagSen[MyMagSen::FD].m_ratio[MyMagSen::LEFT] << " " << MyResource::smartCar().m_servo.m_MagSen[MyMagSen::FD].m_ratio[MyMagSen::RIGHT] << MyLcd::endl
												<< "  HR: "<< MyResource::smartCar().m_servo.m_MagSen[MyMagSen::HD].m_ratio[MyMagSen::LEFT] << " " << MyResource::smartCar().m_servo.m_MagSen[MyMagSen::HD].m_ratio[MyMagSen::RIGHT] << MyLcd::endl;
			break;

		case MyMenu::Pages::Motor:

			break;

		case MyMenu::Pages::Config:
			MyResource::smartCar().m_lcdConsole.setRow(0) << " Motor" << MyLcd::endl << MyLcd::endl

														  << "  Ref:" << MyResource::ConfigTable::MotorConfig::Reference << MyLcd::endl
														  << "  Kp:" << MyResource::ConfigTable::MotorConfig::Kp << MyLcd::endl
//														  << "  Ki:" << MyResource::ConfigTable::MotorConfig::Ki << MyLcd::endl
														  << "  Kd:" << MyResource::ConfigTable::MotorConfig::Kd << MyLcd::endl

														  << " Servo" << MyLcd::endl << MyLcd::endl

														  << "  tKpA:" << MyResource::ConfigTable::ServoConfig::TurningKpA << MyLcd::endl
														  << "  tKd:" << MyResource::ConfigTable::ServoConfig::TurningKd << MyLcd::endl
														  << "  nKp:" << MyResource::ConfigTable::ServoConfig::NormalKp << MyLcd::endl << MyLcd::endl

														  << "  nKp:" << MyResource::ConfigTable::ServoConfig::NormalKp << MyLcd::endl
//														  << "  nKi:" << MyResource::ConfigTable::ServoConfig::NormalKi << MyLcd::endl
														  << "  nKd:" << MyResource::ConfigTable::ServoConfig::NormalKd << MyLcd::endl;
			break;

		case MyMenu::Pages::ComponentsState:
			MyResource::smartCar().m_lcdConsole.setRow(0) << " Components:" << MyLcd::endl << MyLcd::endl

														  << "  ServoEnabled: " << MyResource::smartCar().m_servo.isEnabled() << MyLcd::endl
														  << "  MotorEnabled: " << MyResource::smartCar().m_motor.isEnabled() << MyLcd::endl
														  << "  BuzzerEnabled:" << MyResource::smartCar().m_buzzer.isEnabled() << MyLcd::endl
														  << "  BatteryMeter: " << MyResource::smartCar().m_batteryMeter.getBatteryVoltage() << "V " << MyLcd::endl;
			break;
		}
		MyResource::smartCar().m_lcdConsole.setRow(m_instance->m_curRowsIndex) << " >";
	}
}

void MyMenu::buttonOnPress(const uint8_t id)
{
	switch (id)
	{
	default:
		assert(false);
		break;

	case 0:
		MyResource::smartCar().m_lcdConsole.setEnabled(!MyResource::smartCar().m_lcdConsole.isEnabled());
		break;

	case 1:
		m_instance->m_isChangingCol = !m_instance->m_isChangingCol;
		break;
	}
}

void MyMenu::switchOnCheckedChanged(Switch *pSwitch)
{
	switch (pSwitch->GetPin()->GetName())
	{
	default:
		assert(false);
		break;

	case LIBSC_SWITCH7:

		break;

	case LIBSC_SWITCH6:

		break;

	case LIBSC_SWITCH5:

		break;

	case LIBSC_SWITCH4:

		break;

	case LIBSC_SWITCH3:

		break;

	case LIBSC_SWITCH2:

		break;

	case LIBSC_SWITCH1:

		break;

	case LIBSC_SWITCH0:

		break;
	}
}

void MyMenu::joystickOnChanged(const uint8_t id)
{
	switch ((Joystick::State)id)
	{
	default:
		assert(false);
		break;

	case Joystick::State::kUp:
		m_instance->m_curRowsIndex = inRange(1, m_instance->m_curRowsIndex - 1, m_instance->MAX_ROWS);
		break;

	case Joystick::State::kDown:
		m_instance->m_curRowsIndex = inRange(1, m_instance->m_curRowsIndex + 1, m_instance->MAX_ROWS);
		break;

	case Joystick::State::kLeft:
		m_instance->m_curRowsIndex = inRange(1, m_instance->m_curRowsIndex - 1, m_instance->MAX_ROWS);
		break;

	case Joystick::State::kRight:
		m_instance->m_curRowsIndex = inRange(1, m_instance->m_curRowsIndex - 1, m_instance->MAX_ROWS);
		break;

	case Joystick::State::kSelect:
		m_instance->m_curRowsIndex = inRange(1, m_instance->m_curRowsIndex - 1, m_instance->MAX_ROWS);
		break;
	}
}

void MyMenu::JoystickDataControl(const ControlDirection dir)
{
	switch (dir)
	{
	default:
		assert(false);
		break;

	case ControlDirection::kLeft:
		if (m_isChangingCol)
			MyResource::smartCar().m_lcdConsole.m_lcd->Clear();
		else
			switch (m_curPagesIndex)
			{
			default:
				assert(false);
				break;
			}
		break;

	case ControlDirection::kRight:
		if (m_isChangingCol)
			MyResource::smartCar().m_lcdConsole.m_lcd->Clear();
		else
			switch (m_curPagesIndex)
			{
			default:
				assert(false);
				break;

			}
		break;
	}
}
