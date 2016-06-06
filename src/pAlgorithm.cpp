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
	m_loop.addFunctionToLoop(directionControl, 10);
	m_loop.addFunctionToLoop(speedControl, 100);
	m_loop.addFunctionToLoop(print, 100);
	m_loop.addFunctionToLoop(safetyCheck, 200);
}

void pSmartCar::addVariablesToGrapher(void)
{
//	m_grapher.addWatchedVar(&m_motors[0].getPower(), "Power0");
//	m_grapher.addWatchedVar(&m_motors[1].getPower(), "Power1");
	m_grapher.addWatchedVar(&m_state[StatePos::cur].angle, "Angle");
	m_grapher.addWatchedVar(&m_state[StatePos::cur].dX, "Speed");
	m_grapher.addWatchedVar(&m_state[StatePos::cur].dYaw, "Yaw");
	m_grapher.addWatchedVar(&m_pidOutputVal[PidType::Direction], "DirectionPidOutput");
	m_grapher.addWatchedVar(&m_motors[0].getPower(), "Power");

	m_grapher.addSharedVar(&pResource::configTable.kAngleKp, "AngleKp");
	m_grapher.addSharedVar(&pResource::configTable.kAngleKd, "AngleKd");
	m_grapher.addSharedVar(&pResource::configTable.kDirectionKp, "DirectionKp");
	m_grapher.addSharedVar(&pResource::configTable.kDirectionKd, "DirectionKd");
	m_grapher.addSharedVar(&pResource::configTable.kSpeedKp, "SpeedKp");
	m_grapher.addSharedVar(&pResource::configTable.kSpeedKd, "SpeedKd");

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
	pResource::m_instance->m_oldSpeedPidOuput = pResource::m_instance->m_pidOutputVal[PidType::Speed];
	pResource::m_instance->updatePid(pResource::m_instance->m_state[StatePos::cur].dX, PidType::Speed);
	pResource::m_instance->updateSmoothOutput(pResource::m_instance->m_pidOutputVal[PidType::Speed]);
}

void pSmartCar::print(void)
{
	pResource::m_instance->onDraw();
	pResource::m_instance->m_grapher.sendWatchData();
}

void pSmartCar::safetyCheck(void)
{
	if (!isInRange(pResource::configTable.kAngleMin, pResource::m_instance->m_angle.getAngle(), pResource::configTable.kAngleMax))
		pResource::m_instance->setMotorsEnabled(false);
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
		m_state[StatePos::cur].dX = m_filter.Filter((m_motors[0].getEncoderCount() + m_motors[1].getEncoderCount()) * 0.5f - pResource::configTable.kCountPerDeg * (m_angle.getAngle() - m_state[StatePos::prev].angle));
		m_state[StatePos::cur].dYaw = m_angle.getYawOmega();
	}

	m_state[StatePos::prev].timeStamp = System::Time();
}

void pSmartCar::updateSmoothOutput(const int16_t speed)
{
	m_smoothIncrement = (speed - m_oldSpeedPidOuput) / 20.0f;
}

int16_t	pSmartCar::getSmoothSpeedOutput(void)
{
	return (int16_t)(m_oldSpeedPidOuput += m_smoothIncrement);
}

void pSmartCar::updateMotors(void)
{
	m_motors[0].update();
	m_motors[1].update();
}

void pSmartCar::updateSpeed(void)
{
	m_motors[0].setMappedPower(inRange(-500, m_pidOutputVal[PidType::Angle] + m_pidOutputVal[PidType::Direction] + getSmoothSpeedOutput(), 500));
	m_motors[1].setMappedPower(inRange(-500, m_pidOutputVal[PidType::Angle] - m_pidOutputVal[PidType::Direction] + getSmoothSpeedOutput(), 500));
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
