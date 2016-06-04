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

	reset();

	if (!getConfigTablePtr())
		assert(false);
	else
		readConfig(&configTable, ((ConfigTable *)getConfigTablePtr())->kTableSize);

	if (!configTable.kIsExist)
	{
		setInitialConfigTable();
		saveConfig();
	}
	else
	{
		addConfigToConfigTable();
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

void pResource::setInitialConfigTable(void)
{
	configTable.kIsExist = false;
	configTable.kTableSize = sizeof(ConfigTable);

	configTable.kAccelTruthVal = 0.02f;
	configTable.kCgHeightInM = 0.05f;

	configTable.kIdealAngle = 66.0f;

	configTable.kLeftMotorKp = 300.0f;
	configTable.kLeftMotorKi = 0.0f;
	configTable.kLeftMotorKd = 0.001f;
	configTable.kRightMotorKp = 300.0f;
	configTable.kRightMotorKi = 0.0f;
	configTable.kRightMotorKd = 0.001f;

	configTable.kCountPerDeg = 9560 / 360.0f;
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
