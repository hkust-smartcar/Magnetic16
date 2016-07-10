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
#define UNIQUE_VAL	10

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
		pBuzzer::setEnabled(true);
		pBuzzer::noteDown(32, pBuzzer::defaultValue, pBuzzer::defaultValue, 100);
		pBuzzer::noteDown(48, pBuzzer::defaultValue, 50, 100);
		pBuzzer::setEnabled(false);
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

	configTable.kAccelTruthVal = 0.6f;

	configTable.kIdealAngle = 75.0f;
	configTable.kAngleRange = 30.0f;

	configTable.kAngleKp = 95000.0f;//39150.0f;
	configTable.kAngleKi = 0.0f;
	configTable.kAngleKd = 11.0f;
	configTable.kAngleKq = 0.001f;
	configTable.kAngleKr = 0.999f;

	configTable.kSpeedKp = 3.0f; //3.3f
	configTable.kSpeedKi = 0.08; //1.3f
	configTable.kSpeedKd = 0.0f;
	configTable.kSpeedKq = 0.001f;
	configTable.kSpeedKr = 0.999f;
	configTable.kAccelSpeed = 3.7f;
	configTable.kTargetSpeed = 12.0f;

	configTable.kCountPerRevo = 9557.725f;
}
