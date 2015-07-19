/*
 * MyMagSen.h
 *
 * Author: Peter
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <cassert>
#include <array>
#include <libbase/k60/adc.h>
#include <libsc/timer.h>
#include "MyResource.h"
#include "MyKalmanFilter.h"

using namespace std;
using namespace libbase::k60;
using namespace libsc;

class MyMagSen
{

public:

	enum MagSenId
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

	MyMagSen(MagSenId type);

	void reset(void);

	void init(const float *ratio);

	float update(void);
	float &getOutputValue(void);
	void setOutputSign(bool isSigned);
	array<float, 2> &getFilteredValue(void);
	float &getFilteredValueAvg(void);
	array<float, 2> &getRawValue(void);
	array<float, 2> &getStandardizedValue(void);

	float						m_output;
	array<float, 2>				m_ratio;

private:
	
	MagSenId					m_magSenId;
	array<Adc, 2>				m_magSen;
	array<MyKalmanFilter, 2>	m_filter;
	array<float, 2>				m_rawReading;
	array<float, 2>				m_standardizedReading;
	array<float, 2>				m_filteredReading;
	float						m_avg;
	bool						m_isInit;

	Adc::Config getAdcConfig(MyMagSen::MagSenId type, MyMagSen::Side which);

};
