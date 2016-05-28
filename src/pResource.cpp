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

pResource::ConfigTable *pResource::configTable = nullptr;

pResource::pResource(void)
:
	pSmartCar(),
	m_flash(pFlash::Config(sizeof(pResource::ConfigTable)))
{
	System::Init();

	if (!(configTable = (ConfigTable *)m_flash.getConfigTablePtr()))
		assert(false);
	else if (!configTable->kIsExist)
		setInitialConfigTable();
}

void pResource::setInitialConfigTable(void)
{
	configTable->kIsExist = true;

	configTable->kEncoderCountToMs = 0; // TODO: find const

	// Save
	m_flash.writeConfig();
}
