/*
 * pResource.cpp
 *
 * Author: Peter
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include <libsc/system.h>
#include "pResource.h"

using namespace libsc;

pResource::ConfigTable	*pResource::configTable = (pResource::ConfigTable *)0x7f800; // hard code due to the structure
pResource				*pResource::m_instance = nullptr;

pResource::pResource(void)
:
	pSmartCar(),
	m_flash(pFlash::Config(sizeof(pResource::ConfigTable)))
{
	System::Init();

	if (m_instance)
		assert(false);
	else
		m_instance = this;

//	m_flash.eraseAll();

	if (!(configTable = (ConfigTable *)m_flash.getConfigTablePtr()))
		assert(false);
	else if (configTable->kIsExist)
		setInitialConfigTable();
}

void pResource::setInitialConfigTable(void)
{
	ConfigTable tempTable;
	tempTable.kIsExist = false;

	tempTable.kEncoderCountToCm = 0; // TODO: find const

	tempTable.kIdealAngle = 63.5f;

	tempTable.kLeftMotorKp = 0.001f;
	tempTable.kLeftMotorKi = 0.0f;
	tempTable.kLeftMotorKd = 0.0f;
	tempTable.kRightMotorKp = 0.001f;
	tempTable.kRightMotorKd = 0.0f;
	tempTable.kRightMotorKi = 0.0f;

	// Save
	m_flash.writeConfig(&tempTable);
}
