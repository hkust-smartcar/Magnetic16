/*
 * MyResource.h
 *
 *  Created on: Apr 25, 2015
 *      Author: Peter
 */

#pragma once

#include <array>
#include <cassert>
#include <stdint.h>
#include <libsc/system.h>
#include <libsc/k60/config/2015_magnetic.h>

#define MAX(a, b) ((a > b)? a : b)
#define inRange(n, v, x) ((v < n)? n : ((v > x)? x : v))

class MySmartCar;

class MyResource
{

public:

	class ConfigTable
	{

	public:

		class BuzzerConfig
		{

		public:

			static int WarningPitch;
			static int WarningTimes;

		};

		class MagSenConfig
		{

		public:

			static float Kq;
			static float Kr;

		};

		class ServoConfig
		{

		public:

			static float Reference;
			static float Kp;
			static float Ki;
			static float Kd;

		};

		class MotorConfig
		{

		public:

			static float Reference;
			static float Kp;
			static float Ki;
			static float Kd;

		};

		class BatteryMeterConfig
		{

		public:

			static float MinVoltage;
			static float MaxVoltage;
			static float Ratio;

		};

		class LcdConfig
		{

		public:

			static uint8_t Fps;
			static uint16_t BgColor;
			static uint16_t TxtColor;

			static uint16_t HighPowerColor;
			static uint16_t NormalPowerColor;
			static uint16_t LowPowerColor;

		};

	};

	MyResource(MySmartCar *sc);

	static MySmartCar &smartCar(void);

private:

	MySmartCar *m_smartCar;

};
