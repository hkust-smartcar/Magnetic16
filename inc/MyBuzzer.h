/*
 * MyBuzzer.h
 *
 * Author: Peter
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <libbase/k60/Ftm.h>
#include <libbase/k60/ftm_pwm.h>
#include <libbase/k60/gpio.h>
#include "MyResource.h"

using namespace libbase::k60;

class MyBuzzer
{

public:

	struct note
	{
		char name[5];
		float freq;
	};

	static const note notes[88];

	MyBuzzer(void);

	void noteDown(const uint8_t note, const uint16_t delayMs, const uint8_t times = 0);
	void noteDown(const char noteName[5], const uint16_t delayMs, const uint8_t times = 0);

	void setEnabled(const bool enabled);

	/* Duty Cycle: 0 to 0.4 */
	void setLoudness(const uint8_t percentage);

private:

	uint8_t		m_noteIndex;
	uint8_t		m_loudness;

	Gpo			m_gpo;

//	FtmPwm		m_pwm;

	FtmPwm::Config getFtmConfig(const uint8_t id);
	Gpo::Config getGpoConfig(void);
	uint8_t getNoteIndexByName(const char noteName[5]);

	uint32_t getPeriod(const float freq);
	uint32_t getPosWidth(const uint8_t percentage);

	/* Freq: 0 to 10000 Hz */
	void setNote(const uint8_t note, const uint8_t percentage = 255);
	void setNote(const char noteName[5], const uint8_t percentage = 255);

};
