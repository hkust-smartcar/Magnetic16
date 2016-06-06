/*
 * pAlgorithm.cpp
 *
 * Author: Petel__
 * Copyright (c) 2014-2016 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include <pSmartCar.h>
#include <pResource.h>

using namespace std;
using namespace libsc;

void pSmartCar::addAllRoutineToLoop(void)
{
	m_loop.addFunctionToLoop(update, 5);
	m_loop.addFunctionToLoop(angleControl, 5);
//	m_loop.addFunctionToLoop(directionControl, 10);
//	m_loop.addFunctionToLoop(speedControl, 100);
	m_loop.addFunctionToLoop(print, 100);
	m_loop.addFunctionToLoop(safetyCheck, 500);
}

void pSmartCar::addVariablesToGrapher(void)
{
//	m_grapher.addWatchedVar(&m_motors[0].getPower(), "Power0");
//	m_grapher.addWatchedVar(&m_motors[1].getPower(), "Power1");
	m_grapher.addWatchedVar(&m_state[StatePos::cur].angle, "Angle");
	m_grapher.addWatchedVar(&m_state[StatePos::cur].dX, "Speed");
	m_grapher.addWatchedVar(&m_state[StatePos::cur].dYaw, "Yaw");
	m_grapher.addSharedVar(&pResource::configTable.kAngleKp, "AngleKp");
	m_grapher.addSharedVar(&pResource::configTable.kAngleKd, "AngleKd");

	m_grapher.setOnChangedListener(pResource::grapherOnChangedListener);
}


//========================================================================================
//=========================				Routine				==============================
//========================================================================================
void pSmartCar::update(void)
{
	pResource::m_instance->updateSensors();
	pResource::m_instance->updateState();
	pResource::m_instance->updateMotors();
}

void pSmartCar::angleControl(void)
{
	pResource::m_instance->updatePid(pResource::m_instance->m_state[StatePos::cur].angle, PidType::Angle);
	pResource::m_instance->updateSpeed();
}

void pSmartCar::directionControl(void)
{
	pResource::m_instance->updatePid(pResource::m_instance->m_state[StatePos::cur].dYaw, PidType::Direction);
}

void pSmartCar::speedControl(void)
{
	pResource::m_instance->updatePid(pResource::m_instance->m_state[StatePos::cur].dX, PidType::Speed);
}

void pSmartCar::print(void)
{
	pResource::m_instance->onDraw();
	pResource::m_instance->m_grapher.sendWatchData();
}

void pSmartCar::safetyCheck(void)
{
	if (!isInRange(pResource::configTable.kAngleMin, pResource::m_instance->m_angle.getAngle(), pResource::configTable.kAngleMax))
	{
		pResource::m_instance->m_motors[0].setPower(0);
		pResource::m_instance->m_motors[1].setPower(0);
		pResource::m_instance->setMotorsEnabled(false);
	}
}
//========================================================================================
//========================= | | | | | | | | | | | | | | | | ==============================
//========================= v v v v v v v v v v v v v v v v ==============================


void pSmartCar::updateSensors(void)
{
	m_angle.update();
	m_motors[0].update();
	m_motors[1].update();
}

void pSmartCar::updateState(void)
{
	if (m_state[StatePos::prev].timeStamp)
	{
		m_state[StatePos::prev] = m_state[StatePos::cur];
		m_state[StatePos::cur].angle = pResource::m_instance->m_angle.getAngle();
		m_state[StatePos::cur].dAngle = -pResource::m_instance->m_angle.getOmega(0);
		m_state[StatePos::cur].dX = (m_motors[0].getEncoderCount() + m_motors[1].getEncoderCount()) * 0.5f - pResource::configTable.kCountPerDeg * (m_angle.getAngle() - m_state[StatePos::prev].angle);
		m_state[StatePos::cur].dYaw = m_angle.getYawOmega();
	}

	m_state[StatePos::prev].timeStamp = System::Time();
}

void pSmartCar::updateMotors(void)
{
	m_motors[0].update();
	m_motors[1].update();
}

void pSmartCar::updateSpeed(void)
{
	m_motors[0].setMappedPower(m_pidOutputVal[PidType::Angle] - m_pidOutputVal[PidType::Direction] + m_pidOutputVal[PidType::Speed]);
	m_motors[1].setMappedPower(m_pidOutputVal[PidType::Angle] + m_pidOutputVal[PidType::Direction] + m_pidOutputVal[PidType::Speed]);
}

pSmartCar::State &pSmartCar::State::operator=(const pSmartCar::State &other)
{
	this->angle = other.angle;
	this->dAngle = other.dAngle;
	this->dX = other.dX;
	this->dYaw = other.dYaw;

	return *this;
}

void pSmartCar::onDraw(void)
{
//	m_lcd.setRow(0);
//	m_lcd << m_motors[0].getEncoderCount() << '\t' << m_motors[1].getEncoderCount() << '\t' << endl
//			<< m_motors[0].getPower() << '\t' << m_motors[1].getPower() << '\t' << endl
//			<< m_state[StatePos::cur].dX << '\t' << m_state[StatePos::cur].angle << '\t' << endl
//			<< m_state[StatePos::cur].dYaw << '\t' << m_state[StatePos::cur].dAngle << '\t' << endl;
//			<< m_angle.getAccel(0) << '\t' << m_angle.getAccel(1) << '\t' << m_angle.getAccel(2) << '\t' << endl
//			<< m_angle.getOmega(0) << '\t' << m_angle.getOmega(1) << '\t' << m_angle.getOmega(2) << '\t' << endl
}
