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

Button::Config getButtonConfig(const uint8_t id)
{
	Button::Config config;
	config.id = id;
	config.is_active_low = true;
	config.listener = pSmartCar::onClickListener;
	config.listener_trigger = Button::Config::Trigger::kDown;
	return config;
}

pSmartCar::pSmartCar(void)
:
	m_state(),
	m_loop(),
	m_angle(pAngle::Config(pResource::configTable.kAccelTruthVal, pResource::configTable.kCgHeightInM, true)),
	m_motors{	pMotor(pMotor::Config(1, 0, true, false, leftMotorMapping, pResource::configTable.kLeftMotorKp, pResource::configTable.kLeftMotorKi, pResource::configTable.kLeftMotorKd)),
				pMotor(pMotor::Config(0, 1, false, true, rightMotorMapping, pResource::configTable.kLeftMotorKp, pResource::configTable.kLeftMotorKi, pResource::configTable.kLeftMotorKd)) },
	m_lcd(MiniLcd::Config(0, -1, 30)),
	m_buttons{	Button(getButtonConfig(0)),
				Button(getButtonConfig(1)),
				Button(getButtonConfig(2)) },
	m_grapher(),
	m_motorEnabled(false)
{
	System::Init();
	m_motors[0].setSetPoint(pResource::configTable.kIdealAngle);
	m_motors[1].setSetPoint(pResource::configTable.kIdealAngle);
	m_loop.addFunctionToLoop(update, 5);
	m_loop.addFunctionToLoop(speedControl, 5);
	m_loop.addFunctionToLoop(updateLcd, 100);
	m_loop.addFunctionToLoop(safetyCheck, 500);

//	m_grapher.addWatchedVar(&m_motors[0].getPower(), "Power0");
//	m_grapher.addWatchedVar(&m_motors[1].getPower(), "Power1");
	m_grapher.addWatchedVar(&m_state[StatePos::cur].angle, "Angle");
	m_grapher.addWatchedVar(&m_state[StatePos::cur].dX, "Speed");
	m_grapher.addWatchedVar(&m_state[StatePos::cur].dYaw, "Yaw");
	m_grapher.addSharedVar(&pResource::configTable.kLeftMotorDeadMarginPos, "LPosDead");
	m_grapher.addSharedVar(&pResource::configTable.kLeftMotorDeadMarginNag, "LNagDead");
	m_grapher.addSharedVar(&pResource::configTable.kRightMotorDeadMarginPos, "RPosDead");
	m_grapher.addSharedVar(&pResource::configTable.kRightMotorDeadMarginNag, "RNagDead");

	m_motors[0].reset();
	m_motors[1].reset();

	m_lcd.clear();
}

void pSmartCar::run(void)
{
	m_loop.start();
}

void pSmartCar::onClickListener(const uint8_t id)
{
	switch (id)
	{
	default:
		assert(false);
		// no break

	case 0:
		pResource::m_instance->m_motors[0].reset();
		pResource::m_instance->m_motors[1].reset();
		pResource::m_instance->m_motorEnabled = !pResource::m_instance->m_motorEnabled;
		break;

	case 1:
		pResource::m_instance->m_motors[0].reset();
		DelayMsByTicks(250);
		for (int16_t i = 50; i < 500; i++)
		{
			pResource::m_instance->m_motors[0].setPower(i);
			DelayMsByTicks(100);
			pResource::m_instance->m_motors[0].update();
			if (ABS(pResource::m_instance->m_motors[0].getSpeedCount()) >= 100)
			{
				pResource::configTable.kLeftMotorDeadMarginPos = i;
				break;
			}
		}
		pResource::m_instance->m_motors[0].setPower(0);
		DelayMsByTicks(1000);
		pResource::m_instance->m_motors[0].reset();
		for (int16_t i = -50; i > -500; i--)
		{
			pResource::m_instance->m_motors[0].setPower(i);
			DelayMsByTicks(100);
			pResource::m_instance->m_motors[0].update();
			if (ABS(pResource::m_instance->m_motors[0].getSpeedCount()) >= 100)
			{
				pResource::configTable.kLeftMotorDeadMarginNag = i;
				break;
			}
		}
		pResource::m_instance->saveConfig();
		pResource::m_instance->m_motors[0].setPower(0);
		break;

	case 2:
		pResource::m_instance->m_motors[1].reset();
		DelayMsByTicks(250);
		for (int16_t i = 50; i < 500; i++)
		{
			pResource::m_instance->m_motors[1].setPower(i);
			DelayMsByTicks(100);
			pResource::m_instance->m_motors[1].update();
			if (ABS(pResource::m_instance->m_motors[1].getSpeedCount()) >= 100)
			{
				pResource::configTable.kRightMotorDeadMarginPos = i;
				break;
			}
		}
		pResource::m_instance->m_motors[1].setPower(0);
		DelayMsByTicks(1000);
		pResource::m_instance->m_motors[1].reset();
		for (int16_t i = -50; i > -500; i--)
		{
			pResource::m_instance->m_motors[1].setPower(i);
			DelayMsByTicks(100);
			pResource::m_instance->m_motors[1].update();
			if (ABS(pResource::m_instance->m_motors[1].getSpeedCount()) >= 100)
			{
				pResource::configTable.kRightMotorDeadMarginNag = i;
				break;
			}
		}
		pResource::m_instance->m_motors[1].setPower(0);
		pResource::m_instance->saveConfig();
		break;
	}
}

float pSmartCar::leftMotorMapping(const float val)
{
	if (val == 0.0f)
		return 0.0f;
	else if (val > 0.0f)
		return (val + pResource::configTable.kLeftMotorDeadMarginPos);
	else
		return (val + pResource::configTable.kLeftMotorDeadMarginNag);
}

float pSmartCar::rightMotorMapping(const float val)
{
	if (val == 0.0f)
		return 0.0f;
	else if (val > 0.0f)
		return (val + pResource::configTable.kRightMotorDeadMarginPos);
	else
		return (val + pResource::configTable.kRightMotorDeadMarginNag);
}

void pSmartCar::setMotorsEnabled(const bool enabled)
{
	m_motorEnabled = enabled;
}

void pSmartCar::update(void)
{
	pResource::m_instance->updateSensors();
	pResource::m_instance->updateState();
}

void pSmartCar::speedControl(void)
{

}

void pSmartCar::updateLcd(void)
{
	pResource::m_instance->onDraw();
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

void pSmartCar::updateSensors(void)
{
	m_angle.update();
	if (m_motorEnabled)
	{
		m_motors[0].update(m_angle.getAngle());
		m_motors[1].update(m_angle.getAngle());
	}
	else
	{
		m_motors[0].update();
		m_motors[1].update();
	}
}

void pSmartCar::updateState(void)
{
	if (m_state[StatePos::prev].timeStamp)
	{
		m_state[StatePos::prev] = m_state[StatePos::cur];
		m_state[StatePos::cur].angle = pResource::m_instance->m_angle.getAngle();
		m_state[StatePos::cur].dAngle = -pResource::m_instance->m_angle.getOmega(0);
		m_state[StatePos::cur].dX = (m_motors[0].getSpeedCount() + m_motors[1].getSpeedCount()) * 0.5f - pResource::configTable.kCountPerDeg * (m_angle.getAngle() - m_state[StatePos::prev].angle);
		m_state[StatePos::cur].dYaw = m_angle.getYawOmega();
	}

	m_state[StatePos::prev].timeStamp = System::Time();
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
//	m_lcd << m_motors[0].getSpeedCount() << '\t' << m_motors[1].getSpeedCount() << '\t' << endl
//			<< m_motors[0].getPower() << '\t' << m_motors[1].getPower() << '\t' << endl
//			<< m_state[StatePos::cur].dX << '\t' << m_state[StatePos::cur].angle << '\t' << endl
//			<< m_state[StatePos::cur].dYaw << '\t' << m_state[StatePos::cur].dAngle << '\t' << endl;
//			<< m_angle.getAccel(0) << '\t' << m_angle.getAccel(1) << '\t' << m_angle.getAccel(2) << '\t' << endl
//			<< m_angle.getOmega(0) << '\t' << m_angle.getOmega(1) << '\t' << m_angle.getOmega(2) << '\t' << endl
	m_grapher.sendWatchData();
}
