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
#define UNIQUE_VAL	80

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
	{
		pBuzzer::noteDown(32, pBuzzer::defaultValue, pBuzzer::defaultValue, 100);
		pBuzzer::noteDown(48, pBuzzer::defaultValue, 50, 100);
		reset();
	}

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

	configTable.kBatteryVoltageRatio = 0.3886279f;

	configTable.kAccelTruthVal = 2.0f;
	configTable.kCgHeightInM = 0.05f;

	configTable.kLeftMotorPosConstant = 32.29167f;
	configTable.kRightMotorPosConstant = 45.625f;
	configTable.kLeftMotorNagConstant = 28.58333f;
	configTable.kRightMotorNagConstant = 43.166f;

	configTable.kIdealAngle = 65.0f;
	configTable.kAngleRange = 20.0f;

	configTable.kLeftMotorDeadMarginPos = 300;//70,65;//156;
	configTable.kLeftMotorDeadMarginNag = 300;//65;//164;
	configTable.kRightMotorDeadMarginPos = 300;//20,23;//41;
	configTable.kRightMotorDeadMarginNag = 43;//28;//60;

	configTable.kMotorKp = 0.0f;
	configTable.kMotorKi = 0.0f;
	configTable.kMotorKd = 0.0f;

	configTable.kAngleKp = 202000.0f;
	configTable.kAngleKi = 0.0f;
	configTable.kAngleKd = 17.0f;
	configTable.kAngleKq = 0.001f;
	configTable.kAngleKr = 0.999f;
	configTable.kAngleBeta = 0.12f;

	configTable.kDirectionKp = 12000.0f;
	configTable.kDirectionKi = 0.0f;
	configTable.kDirectionKd = 0.1f;
	configTable.kDirectionKq = 0.001f;
	configTable.kDirectionKr = 0.999f;

	configTable.kSpeedKp = 1.97f;
	configTable.kSpeedKi = 0.0f;
	configTable.kSpeedKd = 0.0f;
	configTable.kSpeedKq = 0.001f;
	configTable.kSpeedKr = 0.999f;

	configTable.kCountPerRevo = 9557.725f;
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
