/*
 * MyResource.h
 *
 * Author: Peter
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <array>
#include <cassert>
#include <stdint.h>
#include <libsc/system.h>
#include <libsc/k60/config/2015_magnetic.h>

#define MAX(a, b) ((a > b)? a : b)
#define inRange(n, v, x) ((v < n)? n : ((v > x)? x : v))
#define ABS(v) ((v > 0)? v : -v)

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

			static float WeightSD;
			static float WeightFD;
			static float WeightHD;

			static uint32_t UpdateFreq;

		};

		class MotorConfig
		{

		public:

			static float Reference;
			static float Kp;
			static float Ki;
			static float Kd;

			static uint32_t UpdateFreq;

		};

		class VarMngConfig
		{

		public:

			static uint16_t UpdateFreq;

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

			static uint16_t UpdateFreq;

		};

	};

	MyResource(MySmartCar *sc);

	static MySmartCar &smartCar(void);

private:

	MySmartCar *m_smartCar;

};
