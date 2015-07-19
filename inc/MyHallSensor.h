/*
 * MyHallSensor.h
 *
 * Author: Peter Lau
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <libbase/k60/gpio.h>
#include "MyResource.h"

using namespace libbase::k60;

class MyHallSensor
{

public:

	explicit MyHallSensor(void);

	void reset(void);
	static void onDetect(Gpi *gpi);
	bool isTheEnd(void);

private:

	Gpi::Config getGpiConfig(void);

	Gpi						m_gpi;
	bool					m_isStarted;
	bool					m_isTheEnd;

	static MyHallSensor		*m_instance;

};
