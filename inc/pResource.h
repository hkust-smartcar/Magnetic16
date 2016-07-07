/*
 * pResource.h
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
#include <libsc/k60/config/2016_magnetic.h>
#include <pFlash.h>
#include <pSmartCar.h>

#define MAX(a, b) ((a > b)? a : b)
#define inRange(n, v, x) ((v < n)? n : ((v > x)? x : v))
#define ABS(v) ((v > 0)? v : -v)

class pResource : private pFlash, public pSmartCar
{

public:

	struct ConfigTable
	{
		bool			kIsExist;
		uint16_t		kTableSize;
		uint8_t			kUniqueVal; // check if the table structure changed

		float			kBatteryVoltageRatio;

		float			kAccelTruthVal;
		float			kCgHeightInM;

		float			kLeftMotorPosConstant;
		float			kRightMotorPosConstant;
		float			kLeftMotorNagConstant;
		float			kRightMotorNagConstant;

		float			kIdealAngle;
		float			kAngleRange;

		int16_t			kLeftMotorDeadMarginPos;
		int16_t			kLeftMotorDeadMarginNag;
		int16_t			kRightMotorDeadMarginPos;
		int16_t			kRightMotorDeadMarginNag;

		float			kSpeedKp;
		float			kSpeedKi;
		float			kSpeedKd;
		float			kSpeedKq;
		float 			kSpeedKr;

		float			kAngleKp;
		float			kAngleKi;
		float			kAngleKd;
		float			kAngleKq;
		float			kAngleKr;
		float			kAngleBeta;

		float			kDirectionKp;
		float			kDirectionKi;
		float			kDirectionKd;
		float			kDirectionKq;
		float			kDirectionKr;

		float			kLiarConst;

		float			kCountPerRevo;
	};

	pResource(void);

	void reset(void);

	void saveConfig(void);

	static void grapherOnChangedListener(void);

	static ConfigTable			configTable;
	static pResource			*m_instance;

private:

	void setInitialConfigTable(void);
	void addConfigToConfigTable(void);

};
