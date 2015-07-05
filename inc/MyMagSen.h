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

	void init(const float *ratio);

	float update(void);
	float &getOutputValue(void);
	void setOutputValue(float newOutput);
	array<float, 2> &getFilteredValue(void);
	float &getFilteredValueAvg(void);
	array<float, 2> &getRawValue(void);
	array<float, 2> &getStandardizedValue(void);

	array<float, 2>				m_ratio;

private:
	
	uint8_t						m_magSenId;
	array<Adc, 2>				m_magSen;
	array<MyKalmanFilter, 2>	m_filter;
	array<float, 2>				m_rawReading;
	array<float, 2>				m_standardizedReading;
	array<float, 2>				m_filteredReading;
	float						m_avg;
	float						m_output;
	bool						m_isInit;

	Adc::Config getAdcConfig(MyMagSen::MagSen type, MyMagSen::Side which);

};
