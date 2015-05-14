/*
 * control.cpp
 *
 * Author: Peter
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include "MyServo.h"
#include "MyMotor.h"
#include "MyPID.h"
#include "MySmartCar.h"
#include "MyVarManager.h"
#include "MyResource.h"

void MyMotor::speedControlRoutine(const uint32_t &timeDelay)
{
	MyResource::smartCar().m_motor.updateSpeed();
}

void MyServo::servoAngleRoutine(const uint32_t &timeDelay)
{
	MyResource::smartCar().m_servo.updateAngle();
//	MyResource::smartCar().m_servo.m_MagSen[0].getValue();
//	MyResource::smartCar().m_servo.m_MagSen[1].getValue();
//	MyResource::smartCar().m_servo.m_MagSen[2].getValue();
}
