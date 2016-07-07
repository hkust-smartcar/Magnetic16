/*
 *	pSmartCar.cpp
 *	
 *	Author: petel__
 *	Copyright (c) 2014-2016 HKUST SmartCar Team
 *	Refer to LICENSE for details
 *	
 */

#include "pSmartCar.h"
#include <libbase/k60/watchdog.h>
#include <pResource.h>

using namespace std;
using namespace libsc;
using namespace libutil;

//Button::Config getButtonConfig(const uint8_t id)
//{
//	Button::Config config;
//	config.id = id;
//	config.is_active_low = true;
//	config.listener = pSmartCar::onClickListener;
//	config.listener_trigger = Button::Config::Trigger::kDown;
//	return config;
//}

pPid::PidParam pSmartCar::getPidConfig(pSmartCar::Type type)
{
	pPid::PidParam param;

	switch (type)
	{
	case 0:
		param.kPFunc =	function<float (float, float, float)>
						(
							[](float error, float additionalInfo, float constant)
							{
								return constant * tan(error * DegToRad);
							}
						);
		param.kP = &pResource::configTable.kAngleKp;
		param.kI = &pResource::configTable.kAngleKi;
		param.kD = &pResource::configTable.kAngleKd;
		param.kDFunc =	function<float (float, float, float)>
						(
							[&](float error, float additionalInfo, float constant)
							{
								return constant /* ABS(m_state[StatePos::cur].dAngle)*/ * m_state[StatePos::cur].dAngle;
							}
						);
		param.setPoint = &pResource::configTable.kIdealAngle;
		param.ignoreRange = 0.0f;
		param.outputMax = 4000;
		param.outputMin = -4000;
		break;

	case 1:
//		param.kPFunc =	function<float (float, float, float)>
//						(
//							[](float error, float additionalInfo, float constant)
//							{
//								return pResource::configTable.kDirectionKp * ABS(error) * error;
//							}
//						);
//		param.kDFunc =	function<float (float, float, float)>
//						(
//							[&](float error, float additionalInfo, float constant)
//							{
//								return constant * -m_state[StatePos::cur].dYaw;
//							}
//						);
		param.kP = &pResource::configTable.kDirectionKp;
		param.kI = &pResource::configTable.kDirectionKi;
		param.kD = &pResource::configTable.kDirectionKd;
		param.setPoint = &m_direction;
		param.ignoreRange = 0.0f;
		param.outputMax = 4000;
		param.outputMin = -4000;
		break;

	case 2:
		param.kP = &pResource::configTable.kSpeedKp;
		param.kI = &pResource::configTable.kSpeedKi;
		param.kD = &pResource::configTable.kSpeedKd;
		param.setPoint = &m_cur_speed;
		param.ignoreRange = 0.0f;
		param.outputMax = 20;
		param.outputMin = -20;
		param.sumMax = 10;
		param.sumMin = -10;
		break;

	case 3:
		param.kP = &pResource::configTable.kDirectionKp;
		param.kI = &pResource::configTable.kDirectionKi;
		param.kD = &pResource::configTable.kDirectionKd;
		param.setPoint = &m_direction;
		param.ignoreRange = 0.0f;
		param.outputMax = 4000;
		param.outputMin = -4000;
		break;
	}

	return param;
}

pFuzzyLogic::Config getFuzzyLogicConfig(void)
{
	pFuzzyLogic::Config config;
	pFuzzyLogic::MembershipFunc errorMF = {	{ -1.0f, -1.0f, -0.82f, -0.5f },			// NL
											{ -0.82f, -0.5f, -0.5f, -0.2f },			// NM
											{ -0.5f, -0.2f, -0.2f, 0.0f },				// NS
											{ -0.1f, 0.0f, 0.0f, 0.1f },				// ZR
											{ 0.0f, 0.2f, 0.2f, 0.5f },					// PS
											{ 0.2f, 0.5f, 0.5f, 0.82f },				// PM
											{ 0.5f, 0.82f, 1.0f, 1.0f } };				// PL

	pFuzzyLogic::MembershipFunc dErrorMF = {{ -200.0f, -200.0f, -175.0f, -130.0f },	// NL
											{ -175.0f, -130.0f, -130.0f, -50.0f },		// NM
											{ -130.0f, -50.0f, -50.0f, 0.0f },			// NS
											{ -50.0f, 0.0f, 0.0f, 50.0f },				// ZR
											{ 0.0f, 50.0f, 50.0f, 130.0f },				// PS
											{ 50.0f, 130.0f, 130.0f, 175.0f },			// PM
											{ 130.0f, 175.0f, 200.0f, 200.0f } };		// PL

	pFuzzyLogic::MembershipFunc outputMF = {{ -4000.0f, -4000.0f, -3500.0f, -2700.0f },	// NL
											{ -3500.0f, -2700.0f, -2700.0f, -1450.0f },	// NM
											{ -2700.0f, -1450.0f, -1450.0f, 0.0f },		// NS
											{ -1450.0f, -100.0f, 100.0f, 1450.0f },		// ZR
											{ 100.0f, 1450.0f, 1450.0f, 2700.0f },		// PS
											{ 1450.0f, 2700.0f, 2700.0f, 3500.0f },		// PM
											{ 2700.0f, 3500.0f, 4000.0f, 4000.0f } };	// PL
	pFuzzyLogic::Rules rules = {	{ 0, 0, 1, 1, 2, 3 },
									{ 0, 1, 1, 2, 3, 4 },
									{ 1, 1, 2, 3, 4, 4 },
									{ 1, 2, 3, 4, 4, 5 },
									{ 2, 3, 4, 4, 5, 6 },
									{ 3, 4, 4, 5, 6, 6 } };
	memcpy(config.errorMembershipFuncs, errorMF, sizeof(pFuzzyLogic::MembershipFunc));
	memcpy(config.dErrorMembershipFuncs, dErrorMF, sizeof(pFuzzyLogic::MembershipFunc));
	memcpy(config.outputMembershipFuncs, outputMF, sizeof(pFuzzyLogic::MembershipFunc));
	memcpy(config.rules, rules, sizeof(pFuzzyLogic::Rules));
	config.approxAccuracy = 1.0f;
	return config;
}

Joystick::Config getJoystickConfig(void)
{
	Joystick::Config config;
	config.id = 0;
	config.is_active_low = true;
	config.dispatcher = [] (const uint8_t id, const Joystick::State which)
						{
							if (!pResource::m_instance->isMotorsEnabled() || pResource::m_instance->isReadyAndSet())
							{
								pBuzzer::noteDown(58);
								pResource::m_instance->setMotorsEnabled(!pResource::m_instance->isMotorsEnabled());
							}
						};
	for (uint8_t i = 0; i < 5; i++)
		config.listener_triggers[i] = Joystick::Config::Trigger::kDown;
	return config;
}

pSmartCar::pSmartCar(void)
:
	m_state(),
	m_direction(0.0f),
	m_ideal_speed(0.0f),
	m_cur_speed(0.0f),
	m_idealAngleOffset(0.0f),
	m_directionOffset(0.0f),
	m_idealAngle(pResource::configTable.kIdealAngle),
	m_batteryVoltage(0.0f),
	m_loop(),
	m_angle(pAngle::Config(pResource::configTable.kAccelTruthVal)),
	m_motors{ pMotor(pMotor::Config(1, 1, true, true, leftMotorMapping)),
				pMotor(pMotor::Config(0, 0, false, false, rightMotorMapping)) },
//	m_lcd(MiniLcd::Config(0, -1, 30)),
//	m_joystick(getJoystickConfig()),
//	m_buttons{	Button(getButtonConfig(0)),
//				Button(getButtonConfig(1)),
//				Button(getButtonConfig(2)) },
	m_leds{ Led({ 3, true }),
			Led({ 2, true }),
			Led({ 1, true }),
			Led({ 0, true }) },
	m_buzzer(),
	m_magSen{ pMagSen(0, true), pMagSen(2, false) },
	m_batmeter({ pResource::configTable.kBatteryVoltageRatio }),
	m_grapher(),
	m_lpf(0.17f),
	m_fuzzyLogic(getFuzzyLogicConfig()),
	m_motorEnabled(false),
	m_pidControllers{	pPid(getPidConfig(Type::Angle)),
						pPid(getPidConfig(Type::Direction)),
						pPid(getPidConfig(Type::Speed)),
						pPid(getPidConfig(Type::SelfDirection)) },
	m_pidOutputVal{ 0 },
	m_filter{	pKalmanFilter(pResource::configTable.kAngleKq, pResource::configTable.kAngleKr, 0, 0.5f),
				pKalmanFilter(pResource::configTable.kDirectionKq, pResource::configTable.kDirectionKr, 0, 0.5f),
				pKalmanFilter(pResource::configTable.kSpeedKq, pResource::configTable.kSpeedKr, 0, 0.5f) },
	m_smoothCounter(0),
	m_smoothIncrement{ 0.0f, 0.0f }
{
	System::Init();

	addAllRoutineToLoop();
	addVariablesToGrapher();
	reset();

	m_batteryVoltage = m_batmeter.GetVoltage();

	m_leds[1].SetEnable(true);

	m_grapher.setOnReceiveListener(onReceive);

	pBuzzer::startSong();
//	pBuzzer::noteDown(48, pBuzzer::defaultValue, pBuzzer::defaultValue, 50);
//	pBuzzer::noteDown(48, pBuzzer::defaultValue, pBuzzer::defaultValue, 50);
//	pBuzzer::noteDown(48, pBuzzer::defaultValue, pBuzzer::defaultValue, 50);

	m_isReadyToRun = true;
}

void pSmartCar::reset(void)
{
	m_direction = 0.0f;
	m_ideal_speed = 0.0f;
	m_motors[0].reset();
	m_motors[1].reset();

//	m_lcd.clear();
}

void pSmartCar::run(void)
{
	if (m_batteryVoltage <= 7.55f)
	{
		pBuzzer::noteDown(40, pBuzzer::defaultValue, 500, 200);
		pBuzzer::noteDown(40, pBuzzer::defaultValue, 500, 200);
		pBuzzer::noteDown(40, pBuzzer::defaultValue, 500, 200);
		assert(false);
	}
	m_loop.start();
}

void pSmartCar::addSpeed(const float speed)
{

	m_cur_speed = inRange(0,m_cur_speed + speed,m_ideal_speed);
}

void pSmartCar::updatePid(const float val, Type type)
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

void pSmartCar::watchDogTimeout(void)
{
	pResource::m_instance->setMotorsEnabled(false);
	for (uint8_t i = 0; i < 4; i++)
		pResource::m_instance->setLed(i, true);
	pBuzzer::noteDown(60, pBuzzer::defaultValue, 1000, 100);
	assert(false);
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
	}
}

float pSmartCar::leftMotorMapping(const float val)
{
	return val;
}

float pSmartCar::rightMotorMapping(const float val)
{
	return val;
}

void pSmartCar::onReceive(const std::vector<Byte>& bytes)
{
	switch (bytes[0])
	{
	case 's':
		pResource::m_instance->setMotorsEnabled(true);
		break;

	case 'd':
		pResource::m_instance->setMotorsEnabled(false);
		pResource::m_instance->m_cur_speed = 0;
		break;

	case '+':
		pResource::m_instance->m_ideal_speed += 1;
		break;

	case '-':
		pResource::m_instance->m_ideal_speed -= 1;
		break;

	case '*':
		pResource::m_instance->m_ideal_speed = 0;
		pResource::m_instance->m_cur_speed = 0;
		pResource::m_instance->m_pidControllers[2].reset();
		break;

	case '0':
		pResource::m_instance->m_grapher.removeAllWatchedVar();
		break;

	case '1':
		pResource::m_instance->m_grapher.addWatchedVar(&pResource::m_instance->m_magSen[0].getVoltage(), "LeftVol");
		pResource::m_instance->m_grapher.addWatchedVar(&pResource::m_instance->m_magSen[0].getVoltage(1), "RightVol");
		break;

	case '2':
		pResource::m_instance->m_grapher.addWatchedVar(&pResource::configTable.kIdealAngle, "IdealAngle");
		pResource::m_instance->m_grapher.addWatchedVar(&pResource::m_instance->m_state[StatePos::cur].angle, "Angle");
		pResource::m_instance->m_grapher.addWatchedVar(&pResource::m_instance->m_pidOutputVal[Type::Angle], "PidAngleOutput");
		break;

	case '3':
		pResource::m_instance->m_grapher.addWatchedVar(&pResource::m_instance->m_angle.getAccelAngle(), "AccelAngle");
		pResource::m_instance->m_grapher.addWatchedVar(&pResource::m_instance->m_angle.getGyroAngle(), "GyroAngle");
		pResource::m_instance->m_grapher.addWatchedVar(&pResource::m_instance->m_state[StatePos::cur].angle, "Angle");
		break;

	case '4':
		pResource::m_instance->m_grapher.addWatchedVar(&pResource::m_instance->m_motors[0].getPower(), "Power0");
		pResource::m_instance->m_grapher.addWatchedVar(&pResource::m_instance->m_motors[1].getPower(), "Power1");
		pResource::m_instance->m_grapher.addWatchedVar(&pResource::m_instance->m_motors[0].getEncoderCount(), "Count0");
		pResource::m_instance->m_grapher.addWatchedVar(&pResource::m_instance->m_motors[1].getEncoderCount(), "Count1");
		break;

	case '5':
		pResource::m_instance->m_grapher.addWatchedVar(&pResource::m_instance->m_state[StatePos::cur].dX, "Speed");
		pResource::m_instance->m_grapher.addWatchedVar(&pResource::m_instance->m_pidControllers[Type::Speed].getSum(), "SpeedSum");
		pResource::m_instance->m_grapher.addWatchedVar(&pResource::m_instance->m_cur_speed, "cur_ideal_speed");
		//		pResource::m_instance->m_grapher.addWatchedVar(&pResource::m_instance->m_state[StatePos::cur].dYaw, "Yaw");
//		pResource::m_instance->m_grapher.addWatchedVar(&pResource::m_instance->m_idealAngleOffset, "angleOffset");
		break;

	case '6':
		pResource::m_instance->m_grapher.addWatchedVar(&pResource::m_instance->m_magSen[0].getResult(), "MAGSEN");
		pResource::m_instance->m_grapher.addWatchedVar(&pResource::m_instance->m_pidOutputVal[Type::Direction], "PidAngleOutput");
		break;

	case '7':
		pResource::m_instance->m_grapher.addWatchedVar(&pResource::m_instance->m_angle.getOmega(0), "Omega[0]");
		pResource::m_instance->m_grapher.addWatchedVar(&pResource::m_instance->m_angle.getOmega(1), "Omega[1]");
		pResource::m_instance->m_grapher.addWatchedVar(&pResource::m_instance->m_angle.getOmega(2), "Omega[2]");

		pResource::m_instance->m_grapher.addWatchedVar(&pResource::m_instance->m_angle.getAccel(0), "Accel[0]");
		pResource::m_instance->m_grapher.addWatchedVar(&pResource::m_instance->m_angle.getAccel(1), "Accel[1]");
		pResource::m_instance->m_grapher.addWatchedVar(&pResource::m_instance->m_angle.getAccel(2), "Accel[2]");

	}
}

bool pSmartCar::isReadyAndSet(void)
{
	return !(m_isReadyToRun = !m_isReadyToRun);
}

void pSmartCar::setMotorsEnabled(const bool enabled)
{
	m_motorEnabled = enabled;
	for (uint8_t i = 0; i < 3; i++)
		m_pidControllers[i].reset();
	m_motors[0].setEnabled(enabled);
	m_motors[1].setEnabled(enabled);
	m_leds[0].SetEnable(enabled);
}

bool pSmartCar::isMotorsEnabled(void)
{
	return m_motorEnabled;
}

void pSmartCar::setMotorPower(const uint8_t index, const int16_t power)
{
	m_motors[index].setPower(power);
}

void pSmartCar::setLed(const uint8_t index, const bool enabled)
{
	m_leds[index].SetEnable(enabled);
}

void pSmartCar::setBeep(const bool isBeep, const uint8_t noteIndex)
{
	m_buzzer.setBeep(true);
}

void pSmartCar::sendDataToGrapher(void)
{
	m_grapher.sendWatchData();
}
