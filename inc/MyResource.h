/*
 * MyResource.h
 *
 *  Created on: Apr 25, 2015
 *      Author: Peter
 */

#pragma once

#include <stdint.h>

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
