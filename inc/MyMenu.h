/*
 * MyMenu.h
 *
 * Author: Peter
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include <cassert>
#include <array>
#include <libbase/k60/gpio.h>
#include <libsc/button.h>
#include <libsc/joystick.h>
#include "MyLcd.h"
#include "MyResource.h"

#pragma once

using namespace std;
using namespace libbase::k60;

class MyMenu
{

public:

	enum struct ControlDirection
	{
		kLeft = 0,
		kRight
	};

	enum struct Pages
	{
		Static = 0,
		Status,
		MagSens,
		Servo,
		Motor,
		Config,
		ComponentsState
	};

	typedef Gpi Switch;

	MyMenu(MyLcd &lcd);

	static void onDraw(const uint32_t &timeDelay);

private:

	Switch::Config getSwitchConfig(const uint8_t id);
	Joystick::Config getJoystickConfig(void);
	static void buttonOnPress(const uint8_t id);
	static void switchOnCheckedChanged(Switch *pSwitch);
	Button::Config getButtonConfig(const uint8_t id);
	static void joystickOnChanged(const uint8_t id);

	void JoystickDataControl(const ControlDirection dir);

	MyLcd						m_lcd;
	array<Button, 2>			m_button;
	array<Switch, 8>			m_switch;
	Joystick					m_joystick;

	bool						m_isChangingCol;
	uint8_t						m_curRowsIndex;
	uint8_t						m_curColsIndex;
	Pages						m_curPagesIndex;
	uint8_t						m_curPageRows;
	const uint8_t				MAX_ROWS = 16;

	static MyMenu				*m_instance;

};
