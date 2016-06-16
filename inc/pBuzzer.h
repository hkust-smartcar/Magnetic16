/*
 * pBuzzer.h
 *
 * Author: Petel__
 * Copyright (c) 2014-2016 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <cassert>
#include <libsc/passive_buzzer.h>

#define inRange(n, v, x) ((v < n)? n : ((v > x)? x : v))

using namespace std;
using namespace libsc;

class pBuzzer
{

public:

	struct note
	{
		char name[5];
		float freq;
	};

	static const note notes[88];

	static const uint16_t defaultValue;

	pBuzzer(void);

	static void setBeep(const bool isBeep);
	static void setBeep(const uint8_t noteIndex = 48, const uint16_t loudness = 333);
	static void noteDown(const uint8_t noteIndex = 48, const uint16_t loudness = 333, const uint16_t delayMs = 100, const uint16_t delayMsAfter = 0);

	static void startSong(void);
	static void endSong(void);

	static pBuzzer		*m_instance;

private:

	PassiveBuzzer		m_buzzer;

};

