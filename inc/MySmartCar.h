/*
 * mySmartCar.h
 *
 *  Created on: Dec 29, 2014
 *      Author: Peter
 */

#pragma once

#include <libsc/system.h>
#include <libsc/led.h>
#include <libsc/alternate_motor.h>
#include <libsc/dir_motor.h>
#include <libsc/tower_pro_mg995.h>
#include <libbase/k60/adc.h>
#include <libsc/k60/ftdi_ft232r.h>
#include <libsc/st7735r.h>
#include <libsc/mma8451q.h>
#include <libsc/futaba_s3010.h>
#include <libsc/trs_d05.h>
#include <libsc/ab_encoder.h>
#include <libsc/button.h>

#include "MyLcd.h"
#include "MyLoop.h"
#include "MyVarManager.h"
#include "MyMotor.h"
#include "MyServo.h"
#include "MyConfig.h"
#include "MyVar.h"

using namespace libsc;
using namespace libsc::k60;
using namespace libbase::k60;
using namespace std;

#define	inRange(n, v, x) ((v > x)? x : ((v < n)? n : v))
#define outRangeOf(v, ov, r) ((v - ov < -r || v - ov > r)? v : 0)
#define abs(v) ((v > 0)? v : -v)
#define length(v) (sizeof(v) / sizeof(v[0]))

#define CMD_FORWARD		0
#define CMD_BACKWARD	1
#define CMD_LEFT		2
#define CMD_RIGHT		3
#define CMD_STOP		4
#define CMD_FRONT		5
#define CMD_RESET		6
#define CMD_SETANGLE	7
#define CMD_SETSPEED	8

class MySmartCar
{

public:

	Button::Config getButtonConfig(const uint8_t id);

	MySmartCar(MyConfig &config, MyVar &vars);

	static void showValue(void);

	void startMainLoop(void);

	void motorSetEnabled(bool enabled);
	void servoSetEnabled(bool enabled);

	void reset(void);

	MyVarManager		myVarMng;

private:

#ifdef LIBSC_USE_BUTTON
	Button				myButtons[2];
#endif

	MyConfig			*myConfig;
	MyVar				*myVars;

	MyLoop				myLoop;
	MyServo				myServo;
	MyMotor				myMotor;
	MyLcd				myLcd;

	MyConfig::SmartCarPowerMode		*m_powerMode;
	MyConfig::SmartCarTurning	*m_turningMode;

	void controlRoutine(void);

	static void onButtonPress(const uint8_t id);

	void doBlink(Byte id);

	bool				m_isStarted;

};
