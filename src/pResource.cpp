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
#define UNIQUE_VAL	150

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

	configTable.kIdealAngle = 75.0f;
	configTable.kAngleRange = 30.0f;

	configTable.kAngleKp = 95000.0f;//39150.0f;
	configTable.kAngleKi = 0.0f;
	configTable.kAngleKd = 11.0f;
	configTable.kAngleKq = 0.001f;
	configTable.kAngleKr = 0.999f;
	configTable.kAngleBeta = 0.12f;

	configTable.kDirectionKp = 5000.0f;
	configTable.kDirectionKi = 0.0f;
	configTable.kDirectionKd = 400.0f;
	configTable.kDirectionKq = 0.001f;
	configTable.kDirectionKr = 0.999f;

	configTable.kSpeedKp = 8.0f; //3.3f
	configTable.kSpeedKi = 0.0f; //1.3f
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
