/*
 * main.cpp
 *
 * Author: Peter
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include <cassert>
#include <cstring>
#include <array>
#include <cmath>
#include <libbase/k60/mcg.h>
#include <libsc/button.h>
#include <libsc/joystick.h>
#include <libsc/k60/ov7725.h>
#include <libbase/k60/gpio.h>
#include <libsc/mpu6050.h>
#include <libsc/dir_motor.h>
#include <libsc/mma8451q.h>
#include <libsc/led.h>
#include <libsc/ab_encoder.h>
#include <libsc/system.h>
#include <libbase/k60/flash.h>
#include <libsc/mini_lcd.h>
#include <pResource.h>
#include <pAngle.h>

namespace libbase
{
	namespace k60
	{

		Mcg::Config Mcg::GetMcgConfig()
		{
			Mcg::Config config;
			config.external_oscillator_khz = 50000;
			config.core_clock_khz = 150000;
			return config;
		}

	}
}

#define PI 3.14159265359f

using namespace std;
using namespace libsc;
using namespace libbase::k60;
using namespace libsc::k60;
using namespace libutil;

//MySmartCar myCar;

int main(void)
{
	System::Init();

	array<Led, 4> leds{ Led({ 0, true }), Led({ 1, true }), Led({ 2, true }), Led({ 3, true }) };

	DirMotor::Config dir0Config, dir1Config;
	dir0Config.id = 0; dir1Config.id = 1;
	array<DirMotor, 2> motors{ DirMotor(dir0Config), DirMotor(dir1Config) };

//	motors[0].SetPower(250); motors[1].SetPower(250);

	function<void (const uint8_t)> buttonsDispatcher = [&](const uint8_t id)
															{
																leds[id].Switch();
															};
	function<void (const uint8_t, const Joystick::State which)> joystickDispatcher = [&](const uint8_t id, const Joystick::State which)
															{
																if ((uint8_t)which + 1 < 5)
																	leds[(uint8_t)which].Switch();
																else
																{
																	leds[0].Switch();
																	leds[3].Switch();
																}
															};

	Button::Config butConfig;
	butConfig.id = 0; butConfig.is_active_low = true; butConfig.listener = buttonsDispatcher; butConfig.listener_trigger = Button::Config::Trigger::kDown;
	Button but0(butConfig); butConfig.id++;
	Button but1(butConfig); butConfig.id++;
	Button but2(butConfig);

	Joystick::Config jsConfig({ 0, true, joystickDispatcher, { 0 }, { Joystick::Config::Trigger::kBoth, Joystick::Config::Trigger::kBoth, Joystick::Config::Trigger::kBoth, Joystick::Config::Trigger::kBoth, Joystick::Config::Trigger::kBoth } });
	Joystick joystick(jsConfig);

//	Mma8451q::Config accelConfig;
//	accelConfig.id = 0;
//	accelConfig.power_mode = Mma8451q::Config::PowerMode::kHighResolution;
//	accelConfig.sensitivity = Mma8451q::Config::Sensitivity::kLow;
//	accelConfig.output_data_rate = Mma8451q::Config::OutputDataRate::k800Hz;
//	Mma8451q accel(accelConfig);
//
//	Mpu6050::Config gyroConfig;
//	gyroConfig.cal_drift = true;
//	gyroConfig.gyro_range = Mpu6050::Config::Range::kMid;
//	Mpu6050 gyro(gyroConfig);

	MiniLcd lcd(MiniLcd::Config(0, -1, 30, 3));
	pAngle angle(pAngle::Config(20, 0.02));

	array<AbEncoder, 2> encoders{ AbEncoder({ 0 }), AbEncoder({ 1 }) };

//	uint8_t index = 0;

	Timer::TimerInt lastTimeLcd = System::Time(), lastTimeAngle = System::Time(), start = 0, end = 0;

	lcd.clear();

	while (true)
	{
		if (System::Time() - lastTimeLcd >= 100)
		{
			lcd.setRow(0);
//			if (gyro.UpdateF() && accel.UpdateF())
//			{
//				array<int16_t, 3> tempAccel = accel.GetAccel();//gyro.GetAccel();
//				array<int32_t, 3> tempOmega = gyro.GetOmega();
//				array<float, 3> tempAccel = accel.GetAccelF();
//				array<float, 3> tempOmega = gyro.GetOmegaF();
				encoders[0].Update(); encoders[1].Update();
				lcd /*<< tempAccel[0] << ", " << (float)tempAccel[1] << ", " << (float)tempAccel[2] << endl
					<< tempOmega[0] << ", " << tempOmega[1] << ", " << tempOmega[2] << endl*/
					<< encoders[0].GetCount() << '\t' << encoders[1].GetCount() << '\t' << endl
					<< motors[0].GetPower() << '\t' << motors[1].GetPower() << '\t' << endl
//					<< tempAccel[0] << '\t' << tempAccel[1] << '\t' << tempAccel[2] << '\t' << endl
//					<< tempOmega[0] << '\t' << tempOmega[1] << '\t' << tempOmega[2] << '\t' << endl;
					<< (angle.getAngle()) << '\t' << endl << (end - start) << endl;
//			}
//			else
//				lcd << "Get Accel Failed!" << endl;

			lastTimeLcd = System::Time();
		}
		else if (System::Time() - lastTimeAngle >= 20)
		{
			start = System::Time();
			angle.update();
			lastTimeAngle = end = System::Time();
		}
//		leds[index].Switch();
//		index = ++index % leds.size();
//		System::DelayMs(100);
	}

}
