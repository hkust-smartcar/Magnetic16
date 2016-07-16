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
	m_loop.addFunctionToLoop(directionControl, 10);
	m_loop.addFunctionToLoop(speedControl, 20);
	m_loop.addFunctionToLoop(angleControl, 5);
	m_loop.addFunctionToLoop(print, 20);
//	m_loop.addFunctionToLoop(safetyCheck, 200);
}

void pSmartCar::addVariablesToGrapher(void)
{
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
	pResource::m_instance->m_magSen[0].updatePair();
}

void pSmartCar::angleControl(void)
{
	pResource::m_instance->updatePid(pResource::m_instance->m_state[StatePos::cur].angle + pResource::m_instance->getSmoothAngleOutput() - ABS(pResource::m_instance->m_state[StatePos::cur].dYaw) * pResource::configTable.kSpinConstant, Type::Angle);

	if (pResource::m_instance->m_motorEnabled && !isInRange2(pResource::m_instance->m_angle.getAngle(), pResource::configTable.kIdealAngle, pResource::configTable.kAngleRange))
	{
		pResource::m_instance->setMotorsEnabled(false);
		pBuzzer::terminated();
		pResource::m_instance->m_isReadyToRun = true;
	}

	pResource::m_instance->updateSpeed();
}

void pSmartCar::directionControl(void)
{
	float tempResult = -pResource::m_instance->m_magSen[0].updatePair();
	pResource::m_instance->updateSmoothDirectionOutput(pResource::m_instance->m_fuzzyLogic.updatePdController(tempResult));
}

void pSmartCar::speedControl(void)
{
//	pResource::m_instance->smoothedIdealSpeed(2.0f);
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


void pSmartCar::smoothedIdealSpeed(const float &accelLimit)
{
	m_curSpeed = inRange(0, m_state[cur].dX + inRange(-m_curSpeed, (m_idealSpeed - m_state[cur].dX) * pResource::configTable.kAccelSpeed, accelLimit), m_idealSpeed);
}

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
		m_state[StatePos::cur].dAngle = -pResource::m_instance->m_angle.getOmega(1);

		float tempDx = m_encoderLpf.filter((m_motors[0].getEncoderCount() + m_motors[1].getEncoderCount()) * 0.0523137f) / (System::Time() - m_state[StatePos::prev].timeStamp);
		if (++m_ignoreSpeedCounter >= 20 || tempDx < m_state[StatePos::cur].dX * 1.5f)
		{
			m_state[StatePos::cur].dX = tempDx;
			m_ignoreSpeedCounter = 0;
		}

		m_state[StatePos::cur].dYaw = m_angle.getYawOmega();
	}

	m_state[StatePos::prev].timeStamp = System::Time();
}

void pSmartCar::updateSmoothAngleOutput(const float newAngle)
{
	m_smoothIncrement[IncrementType::SpeedIncrement] = (newAngle - m_idealAngleOffset) * 0.25f;
}

float pSmartCar::getSmoothAngleOutput(void)
{
	if (isInRange2(m_state[StatePos::cur].angle + m_smoothIncrement[IncrementType::SpeedIncrement] + m_idealAngleOffset, pResource::configTable.kIdealAngle, pResource::configTable.kAngleRange))
		return (m_idealAngleOffset += m_smoothIncrement[IncrementType::SpeedIncrement]);
	else if (isInRange2(m_state[StatePos::cur].angle, pResource::configTable.kIdealAngle, pResource::configTable.kAngleRange))
		return (m_idealAngleOffset = inRange2(m_state[StatePos::cur].angle + m_smoothIncrement[IncrementType::SpeedIncrement] + m_idealAngleOffset, pResource::configTable.kIdealAngle, pResource::configTable.kAngleRange) - pResource::configTable.kIdealAngle);
	else
		return m_idealAngleOffset;
}

void pSmartCar::updateSmoothDirectionOutput(const float newDirection)
{
	m_smoothIncrement[IncrementType::DirectionIncrement] = (newDirection - m_directionOffset) * 0.5f;
}

float pSmartCar::getSmoothDirectionOutput(void)
{
	return (m_directionOffset += m_smoothIncrement[IncrementType::DirectionIncrement]);
}

void pSmartCar::updateMotors(void)
{
	m_motors[0].update();
	m_motors[1].update();
}

void pSmartCar::updateSpeed(void)
{
	float tempDirectionOutput = (m_directionEnabled)? getSmoothDirectionOutput() : 0.0f;
	m_motors[0].setMappedPower(inRange(-9000, m_pidOutputVal[Type::Angle] + tempDirectionOutput, 9000));
	m_motors[1].setMappedPower(inRange(-9000, m_pidOutputVal[Type::Angle] - tempDirectionOutput, 9000));
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
