/*
 * MySmartCar.h
 *
 *  Created on: Apr 22, 2015
 *      Author: Peter
 */

#pragma once

#include <libsc/system.h>
#include "MyLcd.h"
#include "MyBuzzer.h"
#include "MyResource.h"

using namespace libsc;

class MySmartCar
{

public:

//	MyServo			m_servo;
//	MyMotor			m_motor;
//	MyLoop			m_loop;
//	MyVarManager	m_varMng;

	explicit MySmartCar(void);

	MyResource		m_res;
	MyLcd			m_lcdConsole;

private:

};
