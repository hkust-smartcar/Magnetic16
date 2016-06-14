/*
 * pResource.cpp
 *
 * Author: Peter
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include <libsc/system.h>
#include <pResource.h>
#include <pFlash.h>

using namespace libsc;
using namespace std;

// UNIQUE_VAL SHOULD NOT be 255
//
// Change it if u changed the config table
// otherwise the config table in flash memory
// won't update
#define UNIQUE_VAL	99

pResource::ConfigTable	pResource::configTable;
pResource				*pResource::m_instance = nullptr;

pResource::pResource(void)
:
									pFlash(pFlash::Config(&configTable, sizeof(pResource::ConfigTable))),
									pSmartCar()
{
	System::Init();

	if (m_instance)
		assert(false);
	else
		m_instance = this;

	if (UNIQUE_VAL != ((ConfigTable *)getConfigTablePtr())->kUniqueVal)
		reset();

	if (!getConfigTablePtr())
		assert(false);
	else
		readConfig(&configTable, ((ConfigTable *)getConfigTablePtr())->kTableSize);

	if (configTable.kIsExist)
	{
		setInitialConfigTable();
		saveConfig();
	}
	else if (configTable.kTableSize != sizeof(ConfigTable))
	{
		setInitialConfigTable();
		//		addConfigToConfigTable();
		saveConfig();
	}
}

void pResource::reset(void)
{
	eraseAll();
	setInitialConfigTable();
	saveConfig();
}

void pResource::saveConfig(void)
{
	writeConfig(&configTable);
}

void pResource::grapherOnChangedListener(void)
{
	m_instance->saveConfig();
}

void pResource::setInitialConfigTable(void)
{
	configTable.kIsExist = false;
	configTable.kTableSize = sizeof(ConfigTable);
	configTable.kUniqueVal = UNIQUE_VAL;

	configTable.kBatteryVoltageRatio = 0.3895f;

	configTable.kAccelTruthVal = 0.02f;
	configTable.kCgHeightInM = 0.05f;

	//	configTable.kLeftMotorPosConstant = 32.29167f;
	//	configTable.kRightMotorPosConstant = 45.625f;
	configTable.kLeftMotorPosConstant = 0.3229167f;//increase 1 power
	configTable.kRightMotorPosConstant = 0.45625f;
	configTable.kLeftMotorNagConstant = 0.2858333f;
	configTable.kRightMotorNagConstant = 0.43166f;
	configTable.kBatteryConstant = 0.0586022f;

	configTable.kIdealAngle = 62.2f;
	configTable.kAngleMin = 45.0f;
	configTable.kAngleMax = 75.0f;

	configTable.kLeftMotorDeadMarginPos = 0;//104;//156;
	configTable.kLeftMotorDeadMarginNag = 0;//143;//164;
	configTable.kRightMotorDeadMarginPos = 0;//41;
	configTable.kRightMotorDeadMarginNag = 0;//60;

	configTable.kMotorKp = 0.0f;
	configTable.kMotorKi = 0.0f;
	configTable.kMotorKd = 0.0f;

	configTable.kAngleKp = 72.0f;
	configTable.kAngleKi = 0.0f;
	configTable.kAngleKd = 0.018586f;
	configTable.kAngleKq = 0.3f;
	configTable.kAngleKr = 0.9f;

	configTable.kDirectionKp = 0.9f;
	configTable.kDirectionKi = 0.0f;
	configTable.kDirectionKd = 0.01f;
	configTable.kDirectionKq = 0.001f;
	configTable.kDirectionKr = 0.999f;

	configTable.kSpeedKp = 0.0f;
	configTable.kSpeedKi = 0.0f;
	configTable.kSpeedKd = 0.0f;
	configTable.kSpeedKq = 0.001f;
	configTable.kSpeedKr = 0.999f;

	configTable.kCountPerDeg = 9557.725f / 360.0;
}

void pResource::addConfigToConfigTable(void)
{
	ConfigTable tempTable;
	memcpy(&tempTable, &configTable, configTable.kTableSize);
	size_t oldTableSize = tempTable.kTableSize;;
	setInitialConfigTable();
	tempTable.kTableSize = sizeof(ConfigTable);
	memcpy(&configTable, &tempTable, oldTableSize);
}

void pResource::testing()
{

//	stop = 1;
//	set_accel = 0;
	pSmartCar::setMotorsEnabled(true);
	pSmartCar::m_motors[0].update();
	pSmartCar::m_motors[1].update();
	while(true){
		if(!stop){
			pSmartCar::m_motors[0].setAccel(set_accel);
			pSmartCar::m_motors[1].setAccel(set_accel);
			DelayMsByTicks(5);
			pSmartCar::m_motors[0].update();
			pSmartCar::m_motors[1].update();
			cur_speed_l = pSmartCar::m_motors[0].getEncoderCount();
			cur_speed_r = pSmartCar::m_motors[1].getEncoderCount();
			cur_accel_l = cur_speed_l - last_speed_l;
			cur_accel_r = cur_speed_r - last_speed_r;
			pSmartCar::m_grapher.sendWatchData();
		}else{
			pSmartCar::m_motors[0].reset();
			pSmartCar::m_motors[1].reset();
			DelayMsByTicks(5);
			pSmartCar::m_grapher.sendWatchData();
		}
	}
}
