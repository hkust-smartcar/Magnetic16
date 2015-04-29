/*
 * MyServo.h
 *
 *  Created on: Apr 25, 2015
 *      Author: Peter
 */

#include "PIDhandler.h"
#include "MyMagSen.h"
#include "MyServo.h"
#include "MyResource.h"

MyServo::MyServo(void)
:
	TrsD05({ 0 }),
	m_servoPID(&MyResource::ConfigTable::ServoConfig::Reference,
			   &MyResource::ConfigTable::ServoConfig::Kp,
			   &MyResource::ConfigTable::ServoConfig::Ki,
			   &MyResource::ConfigTable::ServoConfig::Kd,
			   -800,
			   800),
	m_MagSen({ MyMagSen(MyMagSen::MagSen::SD),
			   MyMagSen(MyMagSen::MagSen::FD),
			   MyMagSen(MyMagSen::MagSen::HD)})
{}
