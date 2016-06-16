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
#define UNIQUE_VAL	56

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

	configTable.kBatteryVoltageRatio = 0.3886279f;

	configTable.kAccelTruthVal = 0.02f;
	configTable.kCgHeightInM = 0.05f;

	configTable.kLeftMotorPosConstant = 32.29167f;
	configTable.kRightMotorPosConstant = 45.625f;
	configTable.kLeftMotorNagConstant = 28.58333f;
	configTable.kRightMotorNagConstant = 43.166f;

	configTable.kIdealAngle = 65.0f;
	configTable.kAngleMin = 45.0f;
	configTable.kAngleMax = 80.0f;

	configTable.kLeftMotorDeadMarginPos = 0;//104;//156;
	configTable.kLeftMotorDeadMarginNag = 0;//143;//164;
	configTable.kRightMotorDeadMarginPos = 0;//41;
	configTable.kRightMotorDeadMarginNag = 0;//60;

	configTable.kMotorKp = 0.0f;
	configTable.kMotorKi = 0.0f;
	configTable.kMotorKd = 0.0f;

	configTable.kAngleKp = 343.0f;
	configTable.kAngleKi = 0.0f;
	configTable.kAngleKd = 0.0077f;
	configTable.kAngleKq = 0.001f;
	configTable.kAngleKr = 0.999f;
	configTable.kAngleBeta = 0.12f;

	configTable.kDirectionKp = 0.33f;
	configTable.kDirectionKi = 0.0f;
	configTable.kDirectionKd = 0.0f;
	configTable.kDirectionKq = 0.001f;
	configTable.kDirectionKr = 0.999f;

	configTable.kSpeedKp = 0.0f;
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
