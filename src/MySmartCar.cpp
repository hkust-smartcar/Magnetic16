/*
 * MySmarCar.cpp
 *
 *  Created on: Apr 25, 2015
 *      Author: Peter
 */

#include "MyLcd.h"
#include "MySmartCar.h"
#include "MyResource.h"

using namespace libsc;

MySmartCar::MySmartCar(void)
:
	m_res(this),
	m_lcdConsole()
{}
