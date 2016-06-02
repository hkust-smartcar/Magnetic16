/*
 * MyFlash.cpp
 *
 * Author: Peter Lau
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include <libbase/k60/flash.h>
#include <pFlash.h>
#include <pResource.h>

using namespace std;
using namespace libbase::k60;

Flash::Config getFlashConfig(void)
{
	Flash::Config config;
	return config;
}

pFlash::pFlash(Config config)
:
	Flash(getFlashConfig()),
	m_sizeNeeded(config.tableSize)
{
	readConfig(config.tablePtr, m_sizeNeeded);
}

void pFlash::eraseAll(void)
{
	EraseSector(GetStartAddr());
}

void pFlash::readConfig(void *tablePtr, size_t tableSize)
{
	m_tablePtr = (void *)GetStartAddr();
	memcpy(tablePtr, m_tablePtr, tableSize);
}

void pFlash::writeConfig(void)
{
	Write(m_tablePtr, m_sizeNeeded);
}
void pFlash::writeConfig(void *addr)
{
	Write(addr, m_sizeNeeded);
}

void *pFlash::getConfigTablePtr(void) const
{
	return m_tablePtr;
}
