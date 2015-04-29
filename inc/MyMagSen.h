/*
 * MyMagSen.h
 *
 * Author: Peter
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <libbase/k60/adc.h>
#include "MyResource.h"

using namespace std;
using namespace libbase::k60;

class MyMagSen
{

public:

	enum struct MagSen
	{
		SD = 0,
		FD,
		HD
	};

	enum struct Side
	{
		LEFT = 0,
		RIGHT
	};

	MyMagSen(MagSen type);

private:
	
	array<Adc, 2>			m_magSen;

};
