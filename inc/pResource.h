/*
 * pResource.h
 *
 * Author: Peter
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <array>
#include <cassert>
#include <stdint.h>
#include <libsc/system.h>
#include <libsc/k60/config/2016_magnetic.h>
#include "pFlash.h"
#include "pSmartCar.h"

#define MAX(a, b) ((a > b)? a : b)
#define inRange(n, v, x) ((v < n)? n : ((v > x)? x : v))
#define ABS(v) ((v > 0)? v : -v)

class pResource : private pSmartCar
{

public:

	struct ConfigTable
	{
		bool kIsExist;

		uint32_t kEncoderCountToMs;
	};

	pResource(void);

	static ConfigTable			*configTable;

private:

	void setInitialConfigTable(void);

	pFlash					m_flash;

};
