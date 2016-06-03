/*
 *	pSmartCar.cpp
 *	
 *	Author: petel__
 *	Copyright (c) 2014-2016 HKUST SmartCar Team
 *	Refer to LICENSE for details
 *	
 */

#include "pSmartCar.h"
#include <pResource.h>

using namespace libsc;
using namespace libutil;

pSmartCar::pSmartCar(void)
:
	m_state(),
	m_loop(),
	m_angle(pAngle::Config(10, 0.02, true)),
	m_motors{	pMotor(pMotor::Config(1, 0, true, false, leftMotorMapping, pResource::configTable.kLeftMotorKp, pResource::configTable.kLeftMotorKi, pResource::configTable.kLeftMotorKd)),
				pMotor(pMotor::Config(0, 1, false, true, rightMotorMapping, pResource::configTable.kLeftMotorKp, pResource::configTable.kLeftMotorKi, pResource::configTable.kLeftMotorKd)) },
	m_lcd(MiniLcd::Config(0, -1, 30)),
	m_grapher()
{
	System::Init();
	m_motors[0].setSetPoint(pResource::configTable.kIdealAngle);
	m_motors[1].setSetPoint(pResource::configTable.kIdealAngle);
	m_loop.addFunctionToLoop(update, 5);
	m_loop.addFunctionToLoop(updateLcd, 100);

	m_grapher.addWatchedVar(&m_motors[0].getPower(), "power");
	m_grapher.addWatchedVar(&m_motors[0].getSpeedCount(), "count0");
	m_grapher.addWatchedVar(&m_motors[1].getSpeedCount(), "count1");

	m_lcd.clear();
}

void pSmartCar::run(void)
{
	m_loop.start();
}

float pSmartCar::leftMotorMapping(const float val)
{
	if (val == 0.0f)
		return 0.0f;
	else if (val > 0.0f)
		return (val /** 0.014717151f*/ + 155.0f);
	else
		return (val /** 0.018198482f*/ - 110.0f);
}

float pSmartCar::rightMotorMapping(const float val)
{
	if (val == 0.0f)
		return 0.0f;
	else if (val > 0.0f)
		return (val /** 0.0172827379f*/ + 115.0f);
	else
		return (val /** 0.0162533162f*/ - 90.0f);
}

void pSmartCar::update(void)
{
	pResource::m_instance->updateSensors();
	pResource::m_instance->m_state.update();
}

void pSmartCar::updateSensors(void)
{
	m_angle.update();
	m_motors[0].update(m_angle.getAngle());
	m_motors[1].update(m_angle.getAngle());
}

void pSmartCar::State::update()
{
	if (m_lastUpdateTime)
	{
		this->angle = pResource::m_instance->m_angle.getAngle();
		this->dAngle = -pResource::m_instance->m_angle.getOmega(0);
		this->dX += pResource::m_instance->m_angle.getAccel(0) * (System::Time() - m_lastUpdateTime) / 1000;
		this->dYaw = 0;
	}

	m_lastUpdateTime = System::Time();
}

void pSmartCar::balance(void)
{

}

void pSmartCar::updateLcd(void)
{
	pResource::m_instance->onDraw();
}

void pSmartCar::onDraw(void)
{
	m_lcd.setRow(0);
	m_lcd << m_motors[0].getSpeedCount() << '\t' << m_motors[1].getSpeedCount() << '\t' << endl
			<< m_motors[0].getPower() << '\t' << m_motors[1].getPower() << '\t' << endl
			<< m_state.dX << '\t' << m_state.angle << '\t' << endl
			<< m_state.dYaw << '\t' << m_state.dAngle << '\t' << endl;
//			<< m_angle.getAccel(0) << '\t' << m_angle.getAccel(1) << '\t' << m_angle.getAccel(2) << '\t' << endl
//			<< m_angle.getOmega(0) << '\t' << m_angle.getOmega(1) << '\t' << m_angle.getOmega(2) << '\t' << endl
}
