/*
 * mySmartCar.h
 *
 *  Created on: Dec 29, 2014
 *      Author: Peter
 */

#pragma once

#include <libsc/k60/system.h>
#include <libsc/k60/led.h>
#include <libsc/k60/alternate_motor.h>
#include <libsc/k60/dir_motor.h>
#include <libsc/k60/tower_pro_mg995.h>
#include <libbase/k60/adc.h>
#include <libsc/k60/ftdi_ft232r.h>
#include <libsc/k60/st7735r.h>
#include <libsc/k60/mma8451q.h>
#include <libsc/k60/futaba_s3010.h>
#include <libsc/k60/trs_d05.h>
#include <libsc/k60/ab_encoder.h>
#include <libsc/k60/button.h>

#include "MyLcd.h"
#include "MyLoop.h"
#include "MyVarManager.h"
#include "MyMotor.h"
#include "MyServo.h"
#include "MyConfig.h"
#include "MyVar.h"

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

	void reset(void);

	MyVarManager		myVarMng;

private:

#ifdef LIBSC_USE_LED
	Led					myLeds[4];
#endif
#ifdef LIBSC_USE_BUTTON
	Button				myButtons[2];
#endif

	MyConfig			myConfig;

	MyLoop				myLoop;
	MyServo				myServo;
	MyMotor				myMotor;

	MyLcd				myLcd;

	MyConfig::SmartCarPowerMode		*m_powerMode;
	MyConfig::SmartCarTurningMode	*m_turningMode;

	void controlRoutine(void);

	static void onButtonPress(const uint8_t id);

	void doBlink(Byte id);

	bool				m_isStarted;

};
