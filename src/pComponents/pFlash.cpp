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

using namespace libbase::k60;

pFlash			*pFlash::m_instance = nullptr;

Flash::Config getFlashConfig(void)
{
	Flash::Config config;
	return config;
}

pFlash::pFlash(void)
:
	Flash(getFlashConfig()),
	m_sizeNeeded(0)
{
	if (!m_instance)
		m_instance = this;

	m_sizeNeeded = (uint32_t)&MyResource::ConfigTable::LcdConfig::UpdateFreq + sizeof(MyResource::ConfigTable::LcdConfig::UpdateFreq) -
						(uint32_t)&MyResource::ConfigTable::MagSenConfig::Kq;

	readConfig();
}

void pFlash::eraseAll(void)
{
	EraseSector(GetStartAddr());
}

void pFlash::readConfig(void)
{
	if (*((uint8_t *)(GetStartAddr() + m_sizeNeeded - 1)) == 0xFF || *((uint8_t *)GetStartAddr()) == 0xFF || *((uint8_t *)(GetStartAddr() + m_sizeNeeded)) != 0xFF)
		return ;

	assert(Read(&MyResource::ConfigTable::MagSenConfig::Kq, m_sizeNeeded) == FlashStatus::kSuccess);
}

void pFlash::writeConfig(void)
{
	m_instance->Write(&MyResource::ConfigTable::MagSenConfig::Kq, m_instance->m_sizeNeeded);
}
