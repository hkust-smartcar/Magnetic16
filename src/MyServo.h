/*
 * MyServo.h
 *
 *  Created on: Apr 25, 2015
 *      Author: Peter
 */

#include "MyServo.h"
#include "MyResource.h"

MyServo::MyServo(void)
:
	m_servoPID(MyResource::ConfigTable::ServoConfig::Reference,
			   MyResource::ConfigTable::ServoConfig::Kp,
			   MyResource::ConfigTable::ServoConfig::Ki,
			   MyResource::ConfigTable::ServoConfig::Kd,
			   -800,
			   800),
	m_MagSen({ MyMagSen(MagSen::SD),
			   MyMagSen(MagSen::FD),
			   MyMagSen(MagSen::HD)})
{}
