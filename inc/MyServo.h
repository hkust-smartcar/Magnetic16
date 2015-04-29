/*
 * MyServo.h
 *
 *  Created on: Apr 25, 2015
 *      Author: Peter
 */

#pragma once

#include <libbase/k60/adc.h>
#include <libsc/trs_d05.h>
#include "MyMagSen.h"
#include "PIDhandler.h"
#include "MyResource.h"

using namespace libsc;
using namespace libbase::k60;
using namespace std;

class MyServo : private TrsD05
{

public:

	array<MyMagSen, 3>		m_MagSen;

	MyServo(void);

private:

	PIDhandler				m_servoPID;

};
