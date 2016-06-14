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
			return constant * m_state[StatePos::cur].dAngle;
				}
		);
		param.setPoint = &m_idealAngle;
		param.ignoreRange = 0.2f;
		param.outputMax = 400;
		param.outputMin = -400;
		break;

	case 1:
		param.kP = &pResource::configTable.kDirectionKp;
		param.kI = &pResource::configTable.kDirectionKi;
		param.kD = &pResource::configTable.kDirectionKd;
		param.setPoint = &m_direction;
		param.ignoreRange = 12.0f;
		param.outputMax = 400;
		param.outputMin = -400;
		break;

	case 2:
		param.kP = &pResource::configTable.kSpeedKp;
		param.kI = &pResource::configTable.kSpeedKi;
		param.kD = &pResource::configTable.kSpeedKd;
		param.setPoint = &m_speed;
		param.ignoreRange = 0.0f;
		param.outputMax = 500;
		param.outputMin = -500;
		param.sumMax = 200;
		param.sumMin = 200;
	}

	return param;
}

pSmartCar::pSmartCar(void)
:
					m_state(),
					m_direction(0.0f),
					m_speed(0.0f),
					m_idealAngleOffset(pResource::configTable.kIdealAngle),
					m_idealAngle(pResource::configTable.kIdealAngle),
					m_batteryVoltage(0.0f),
					m_loop(),
					m_angle(pAngle::Config(pResource::configTable.kAccelTruthVal, pResource::configTable.kCgHeightInM)),
					m_motors{ pMotor(pMotor::Config(1, 0, true, false, leftMotorMapping)),
	pMotor(pMotor::Config(0, 1, false, true, rightMotorMapping)) },
	m_lcd(MiniLcd::Config(0, -1, 30)),
	m_buttons{	Button(getButtonConfig(0)),
		Button(getButtonConfig(1)),
		Button(getButtonConfig(2)) },
		m_leds{ Led({ 3, true }),
			Led({ 2, true }),
			Led({ 1, true }),
			Led({ 0, true }) },
			m_buzzer({ 0, false }),
			m_batmeter({ pResource::configTable.kBatteryVoltageRatio }),
			m_grapher(),
			m_motorEnabled(false),
			m_pidControllers{	pPid(getPidConfig(Type::Angle)),
				pPid(getPidConfig(Type::Direction)),
				pPid(getPidConfig(Type::Speed)) },
				m_pidOutputVal{ 0 },
				m_filter{	pKalmanFilter(pResource::configTable.kAngleKq, pResource::configTable.kAngleKr, 0, 0.5f),
					pKalmanFilter(pResource::configTable.kDirectionKq, pResource::configTable.kDirectionKr, 0, 0.5f),
					pKalmanFilter(pResource::configTable.kSpeedKq, pResource::configTable.kSpeedKr, 0, 0.5f) },
					m_smoothCounter(0),
					m_smoothIncrement(0),
					set_accel(0),cur_accel_l(0),last_speed_l(0),cur_speed_l(0),
					cur_accel_r(0),last_speed_r(0),cur_speed_r(0),stop(0)
					{
						System::Init();

						addAllRoutineToLoop();
						addVariablesToGrapher();
						reset();

						m_batteryVoltage = m_batmeter.GetVoltage();

						m_leds[1].SetEnable(true);
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
						else{
							return (val/pResource::configTable.kLeftMotorPosConstant);
						}
//							else if (val > 0.0f)
//								return val * pResource::configTable.kLeftMotorPosConstant + pResource::configTable.kLeftMotorDeadMarginPos;
//							else
//								return (val * pResource::configTable.kLeftMotorNagConstant - pResource::configTable.kLeftMotorDeadMarginNag);

					}

					float pSmartCar::rightMotorMapping(const float val)
					{
						if (val == 0.0f)
							return 0.0f;
						else{
							return (val/pResource::configTable.kRightMotorPosConstant);
						}
//							else if (val > 0.0f)
//								return val * pResource::configTable.kRightMotorPosConstant + pResource::configTable.kRightMotorDeadMarginPos;
//							else
//								return val * pResource::configTable.kRightMotorNagConstant - pResource::configTable.kRightMotorDeadMarginNag;

					}

					void pSmartCar::setMotorsEnabled(const bool enabled)
					{
						m_motorEnabled = enabled;
						m_motors[0].setEnabled(enabled);
						m_motors[1].setEnabled(enabled);
						m_leds[0].SetEnable(enabled);
					}

					void pSmartCar::setMotorPower(const uint8_t index, const int16_t power)
					{
						m_motors[index].setPower(power);
					}

					void pSmartCar::setLed(const uint8_t index, const bool enabled)
					{
						m_leds[index].SetEnable(enabled);
					}

					void pSmartCar::sendDataToGrapher(void)
					{
						m_grapher.sendWatchData();
					}
