/*
 * MyMotor.cpp
 *
 * Author: Peter
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include <libsc/dir_motor.h>
#include <libsc/ab_encoder.h>
#include "MyMotor.h"
#include "MyResource.h"

using namespace libsc;

DirMotor::Config getMotorConfig(uint8_t id)
{
	DirMotor::Config config;
	config.id = id;
	return config;
}

MyMotor::MyMotor(void)
:
	DirMotor(getMotorConfig(0)),
	m_encoder()
{}

void setSpeed(int16_t speed)
{

}
