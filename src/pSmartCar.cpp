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

using namespace std;
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

pPid::PidParam pSmartCar::getPidConfig(pSmartCar::PidType type)
{
	pPid::PidParam param;

	switch (type)
	{
	case 0:
		param.kPFunc =	function<float (float)>
						(
							[](float error)
							{
								if (ABS(error) < 0.3f)
									return 0.0f;
								else
									return pResource::configTable.kAngleKp * sin(error * DegToRad);
							}
						);
		param.kI = &pResource::configTable.kAngleKi;
		param.kD = &pResource::configTable.kAngleKd;
		param.setPoint = &pResource::configTable.kIdealAngle;
		param.ignoreRange = 0.5f;
		param.max = 500;
		param.min = -500;
		break;

	case 1:
		param.kP = &pResource::configTable.kDirectionKp;
		param.kI = &pResource::configTable.kDirectionKi;
		param.kD = &pResource::configTable.kDirectionKd;
		param.setPoint = &m_direction;
		param.ignoreRange = 10.0f;
		param.max = 200;
		param.min = -200;
		break;

	case 2:
		param.kP = &pResource::configTable.kSpeedKp;
		param.kI = &pResource::configTable.kSpeedKi;
		param.kD = &pResource::configTable.kSpeedKd;
		param.setPoint = &m_speed;
		param.ignoreRange = 15.0f;
		param.max = 500;
		param.min = -500;
	}

	return param;
}

pSmartCar::pSmartCar(void)
:
	m_state(),
	m_direction(0.0f),
	m_speed(0.0f),
	m_loop(),
	m_angle(pAngle::Config(pResource::configTable.kAccelTruthVal, pResource::configTable.kCgHeightInM)),
	m_motors{ pMotor(pMotor::Config(1, 0, true, false, leftMotorMapping)),
				pMotor(pMotor::Config(0, 1, false, true, rightMotorMapping)) },
	m_lcd(MiniLcd::Config(0, -1, 30)),
	m_buttons{	Button(getButtonConfig(0)),
				Button(getButtonConfig(1)),
				Button(getButtonConfig(2)) },
	m_grapher(),
	m_motorEnabled(false),
	m_pidControllers{	pPid(getPidConfig(PidType::Angle)),
						pPid(getPidConfig(PidType::Direction)),
						pPid(getPidConfig(PidType::Speed)) },
	m_pidOutputVal{ 0 },
	m_filter(pResource::configTable.kKalmanKq, pResource::configTable.kKalmanKr, 0, 0.5),
	m_oldSpeedPidOuput(0),
	m_smoothCounter(0),
	m_smoothIncrement(0)
{
	System::Init();

	addAllRoutineToLoop();
	addVariablesToGrapher();
	reset();
}

void pSmartCar::reset(void)
{
	m_direction = 0.0f;
	m_speed = 0.0f;
	m_motors[0].reset();
	m_motors[1].reset();

	m_lcd.clear();
}

void pSmartCar::run(void)
{
	m_loop.start();
}

void pSmartCar::updatePid(const float val, PidType type)
{
	m_pidOutputVal[type] = m_pidControllers[type].getOutput(val);
}

pSmartCar::State &pSmartCar::State::operator=(const pSmartCar::State &other)
{
	this->angle = other.angle;
	this->dAngle = other.dAngle;
	this->dX = other.dX;
	this->dYaw = other.dYaw;

	return *this;
}

void pSmartCar::onClickListener(const uint8_t id)
{
	switch (id)
	{
	default:
		assert(false);
		// no break

	case 0:
		pResource::m_instance->setMotorsEnabled(pResource::m_instance->m_motorEnabled = !pResource::m_instance->m_motorEnabled);
		break;

	case 1:
		pResource::m_instance->m_motors[0].reset();
		DelayMsByTicks(250);
		for (int16_t i = 50; i < 500; i++)
		{
			pResource::m_instance->m_motors[0].setPower(i);
			DelayMsByTicks(100);
			pResource::m_instance->m_motors[0].update();
			if (ABS(pResource::m_instance->m_motors[0].getEncoderCount()) >= 100)
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
			if (ABS(pResource::m_instance->m_motors[0].getEncoderCount()) >= 100)
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
			if (ABS(pResource::m_instance->m_motors[1].getEncoderCount()) >= 100)
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
			if (ABS(pResource::m_instance->m_motors[1].getEncoderCount()) >= 100)
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
		return (val - pResource::configTable.kLeftMotorDeadMarginNag);
}

float pSmartCar::rightMotorMapping(const float val)
{
	if (val == 0.0f)
		return 0.0f;
	else if (val > 0.0f)
		return (val + pResource::configTable.kRightMotorDeadMarginPos);
	else
		return (val - pResource::configTable.kRightMotorDeadMarginNag);
}

void pSmartCar::setMotorsEnabled(const bool enabled)
{
	m_motorEnabled = enabled;
	m_motors[0].setEnabled(enabled);
	m_motors[1].setEnabled(enabled);
}
