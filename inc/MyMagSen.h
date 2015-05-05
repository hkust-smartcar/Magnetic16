/*
 * MyMagSen.h
 *
 * Author: Peter
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <array>
#include <libbase/k60/adc.h>
#include "MyResource.h"
#include "MyKalmanFilter.h"

using namespace std;
using namespace libbase::k60;

class MyMagSen
{

public:

	enum MagSen
	{
		SD = 0,
		FD,
		HD
	};

	enum Side
	{
		LEFT = 0,
		RIGHT
	};

	MyMagSen(MagSen type);

	void reset(void);

	float getValue(void);
	array<float, 2> &getRawValue(void);

private:
	
	array<Adc, 2>				m_magSen;
	array<MyKalmanFilter, 2>	m_filter;
	array<float, 2>				m_rawReading;
	float						m_reading;
};
