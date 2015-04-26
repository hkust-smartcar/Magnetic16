/*
 * MyServo.h
 *
 *  Created on: Apr 25, 2015
 *      Author: Peter
 */

#pragma once

#include <array>
#include <libbase/k60/adc.h>
#include <libsc/trs_d05.h>
#include "PIDhandler.h"

using namespace libsc;
using namespace libbase::k60;
using namespace std;

class MyServo : TrsD05
{

public:

	class MyMagSen : Adc
	{

	public:

		MyMagSen(void);

	private:



	};

	enum struct MagSen
	{
		SD = 0,
		FD,
		HD
	};
	array<MyMagSen, 3>		m_MagSen;

	MyServo(void);

private:

	PIDhandler				m_servoPID;

};
