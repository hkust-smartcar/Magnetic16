/*
 * pAlgorithm.cpp
 *
 * Author: Petel__
 * Copyright (c) 2014-2016 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include <pSmartCar.h>
#include <libbase/k60/watchdog.h>
#include <pResource.h>

using namespace std;
using namespace libsc;
using namespace libbase::k60;

void pSmartCar::addAllRoutineToLoop(void)
{
	m_loop.addFunctionToLoop(update, 5);
	m_loop.addFunctionToLoop(angleControl, 5);
	m_loop.addFunctionToLoop(directionControl, 10);
	m_loop.addFunctionToLoop(speedControl, 100);
	m_loop.addFunctionToLoop(print, 20);
//	m_loop.addFunctionToLoop(safetyCheck, 200);
}

void pSmartCar::addVariablesToGrapher(void)
{
//	m_grapher.addWatchedVar(&m_angle.getOmega(0), "Omega[0]");
//	m_grapher.addWatchedVar(&m_angle.getOmega(1), "Omega[1]");
//	m_grapher.addWatchedVar(&m_angle.getOmega(2), "Omega[2]");
//
//	m_grapher.addWatchedVar(&m_angle.getAccel(0), "Accel[0]");
//	m_grapher.addWatchedVar(&m_angle.getAccel(1), "Accel[1]");
//	m_grapher.addWatchedVar(&m_angle.getAccel(2), "Accel[2]");

//	m_grapher.addWatchedVar(&m_motors[0].getEncoderCount(), "Count0");
//	m_grapher.addWatchedVar(&m_motors[1].getEncoderCount(), "Count1");
	m_grapher.addWatchedVar(&pResource::configTable.kIdealAngle, "IdealAngle");
//	m_grapher.addWatchedVar(&m_state[StatePos::cur].dAngle, "dAngle");
	m_grapher.addWatchedVar(&m_state[StatePos::cur].angle, "Angle");
	m_grapher.addWatchedVar(&m_state[StatePos::cur].dX, "Speed");
	m_grapher.addWatchedVar(&m_state[StatePos::cur].dYaw, "Yaw");
//	m_grapher.addWatchedVar(&m_pidOutputVal[Type::Angle], "PidAngleOutput");
////	m_grapher.addWatchedVar(&m_pidOutputVal[Type::Direction], "PidDirectionOutput");
////	m_grapher.addWatchedVar(&m_pidOutputVal[Type::Speed], "PidSpeedOutput");
//	m_grapher.addWatchedVar(&m_state[StatePos::cur].dX, "Speed");
//	m_grapher.addWatchedVar(&m_idealAngleOffset, "angleOffset");
//	m_grapher.addWatchedVar(&m_pidControllers[Type::Speed].getSum(), "Sum");
//	m_grapher.addWatchedVar(&m_motors[0].getPower(), "Power0");
//	m_grapher.addWatchedVar(&m_motors[1].getPower(), "Power1");
////	m_grapher.addWatchedVar(&m_batteryVoltage, "BatVol");
//	m_grapher.addWatchedVar(&m_magSen[0].getVoltage(0), "LeftVol");
//	m_grapher.addWatchedVar(&m_magSen[0].getVoltage(1), "RightVol");

	m_grapher.addWatchedVar(&m_angle.getAccelAngle(), "AccelAngle");
	m_grapher.addWatchedVar(&m_angle.getGyroAngle(), "GyroAngle");
	m_grapher.addWatchedVar(&m_state[StatePos::cur].angle, "Angle");

	m_grapher.addSharedVar(&pResource::configTable.kAngleKp, "AngleKp");
	m_grapher.addSharedVar(&pResource::configTable.kAngleKd, "AngleKd");
	m_grapher.addSharedVar(&pResource::configTable.kDirectionKp, "DirectionKp");
//	m_grapher.addSharedVar(&pResource::configTable.kDirectionKd, "DirectionKd");
//	m_grapher.addSharedVar(&pResource::configTable.kDirectionKi, "DirectionKi");
	m_grapher.addSharedVar(&m_speed, "SpeedSetPoint");
	m_grapher.addSharedVar(&pResource::configTable.kSpeedKp, "SpeedKp");
	m_grapher.addSharedVar(&pResource::configTable.kSpeedKd, "SpeedKd");
	m_grapher.addSharedVar(&pResource::configTable.kSpeedKi, "SpeedKi");
//	m_grapher.addSharedVar(&pResource::configTable.kAngleKq, "AngleKq");
//	m_grapher.addSharedVar(&pResource::configTable.kAngleKr, "AngleKr");
//	m_grapher.addSharedVar(&pResource::configTable.kIdealAngle, "IdealAngle");
	m_grapher.addSharedVar(&pResource::configTable.kAccelTruthVal, "TrustVal");

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
	Watchdog::Feed();
}

void pSmartCar::angleControl(void)
{
	pResource::m_instance->updatePid(pResource::m_instance->m_state[StatePos::cur].angle + pResource::m_instance->getSmoothAngleOutput(), Type::Angle);
	if (pResource::m_instance->m_motorEnabled && !isInRange(pResource::configTable.kAngleMin, pResource::m_instance->m_angle.getAngle(), pResource::configTable.kAngleMax))
	{
		pResource::m_instance->setMotorsEnabled(false);
		pBuzzer::endSong();
		pResource::m_instance->m_isReadyToRun = true;
	}
	pResource::m_instance->updatePid(pResource::m_instance->m_state[StatePos::cur].dYaw, Type::Direction);
	pResource::m_instance->updateSpeed();
}

void pSmartCar::directionControl(void)
{
	float tempResult = pResource::m_instance->m_magSen[0].update();
	// Fuzzy Logic
	pResource::m_instance->updatePid(tempResult, Type::Direction);
}

void pSmartCar::speedControl(void)
{
	pResource::m_instance->updatePid(pResource::m_instance->m_state[StatePos::cur].dX, Type::Speed);
	pResource::m_instance->updateSmoothAngleOutput(pResource::m_instance->m_pidOutputVal[Type::Speed]);
}

void pSmartCar::print(void)
{
//	pResource::m_instance->onDraw();
	pResource::m_instance->m_grapher.sendWatchData();
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
		m_state[StatePos::cur].angle = m_filter[Type::Angle].filter(pResource::m_instance->m_angle.getAngle());
		m_state[StatePos::cur].dAngle = -pResource::m_instance->m_angle.getOmega(0);
		m_state[StatePos::cur].dX = m_filter[Type::Speed].filter((m_motors[0].getEncoderCount() + m_motors[1].getEncoderCount()) * 0.0523137f / (System::Time() - m_state[StatePos::prev].timeStamp));//m_filter.Filter((m_motors[0].getEncoderCount() + m_motors[1].getEncoderCount()) * 0.5f - pResource::configTable.kCountPerDeg * (m_angle.getAngle() - m_state[StatePos::prev].angle));
		m_state[StatePos::cur].dYaw = m_filter[Type::Direction].filter(m_angle.getYawOmega());
	}

	m_state[StatePos::prev].timeStamp = System::Time();
}

void pSmartCar::updateSmoothAngleOutput(const float newAngle)
{
	m_smoothIncrement = (newAngle - m_idealAngleOffset) * 0.05f;
}

float pSmartCar::getSmoothAngleOutput(void)
{
	return (m_idealAngleOffset += m_smoothIncrement);
}

void pSmartCar::updateMotors(void)
{
	m_motors[0].update();
	m_motors[1].update();
}

void pSmartCar::updateSpeed(void)
{
	m_motors[0].setMappedPower(inRange(-400, m_pidOutputVal[Type::Angle] + m_pidOutputVal[Type::Direction], 400));
	m_motors[1].setMappedPower(inRange(-400, m_pidOutputVal[Type::Angle] - m_pidOutputVal[Type::Direction], 400));
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
