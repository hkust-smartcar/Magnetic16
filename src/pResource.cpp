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
#define UNIQUE_VAL	5

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

	configTable.kAccelTruthVal = 1.8f;

	configTable.kIdealAngle = 77.5f;
	configTable.kRunAngle = 73.0f;
	configTable.kAngleRange = 30.0f;

	configTable.kAngleKp = 50000.0f;
	configTable.kAngleKi = 0.0f;
	configTable.kAngleKd = 21.0f;

	configTable.kSpeedKp = 2.2f;
	configTable.kSpeedKi = 0.0f;
	configTable.kSpeedKd = 0.0f;
	configTable.kSpinConstant = 0.01f;
	configTable.kTargetSpeed = 6.0f;

	configTable.kErrorMfLimit = 1.0f;
	configTable.kErrorMfL = 0.81f;
	configTable.kErrorMfM = 0.77f;
	configTable.kErrorMfS = 0.59f;
	configTable.kErrorMfZ = 0.0f;

	configTable.kDerrorMfLimit = 200.0f;
	configTable.kDerrorMfL = 5.8f;
	configTable.kDerrorMfM = 4.2f;
	configTable.kDerrorMfS = 2.9f;
	configTable.kDerrorMfZ = 0.0f;

	configTable.kCountPerRevo = 9557.725f;
}


