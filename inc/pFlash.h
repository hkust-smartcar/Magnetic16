/*
 * MyFlash.h
 *
 * Author: Peter Lau
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <cassert>
#include <libbase/helper.h>
#include <libbase/k60/flash.h>

using namespace libbase::k60;

class pFlash : public Flash
{

public:

	struct Config
	{
		void			*tablePtr;
		uint16_t	tableSize;

		Config(void *_tablePtr, uint16_t _tableSize)
		:
			tablePtr(_tablePtr),
			tableSize(_tableSize)
		{}
	};

	pFlash(Config config);

	void eraseAll(void);

	void readConfig(void* tablePtr, size_t tableSize);
	void writeConfig(void);
	void writeConfig(void *addr);

	void changeSize(size_t tableSize);

	void *getConfigTablePtr(void) const;

private:

	void				*m_tablePtr;
	size_t			m_sizeNeeded;

};
