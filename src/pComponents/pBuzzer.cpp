/*
 * pBuzzer.cpp
 *
 * Author: Petel__
 * Copyright (c) 2014-2016 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include <pBuzzer.h>
#include <pLoop.h>

using namespace std;
using namespace libsc;

#define isDefault(v) (v == (uint16_t)-1)

pBuzzer *pBuzzer::m_instance = nullptr;
const uint16_t pBuzzer::defaultValue = (uint16_t)-1;

PassiveBuzzer::Config getBuzzerConfig(void)
{
	PassiveBuzzer::Config config;
	config.id = 0;
	config.pos = 0;
	config.freq = (uint32_t)pBuzzer::notes[40].freq;
	return config;
}

pBuzzer::pBuzzer(void)
:
	m_buzzer(getBuzzerConfig()),
	m_enabled(false)
{
	assert(!m_instance);
	m_instance = this;
}

void pBuzzer::setEnabled(const bool enabled)
{
	m_instance->m_enabled = enabled;
}

bool pBuzzer::getEnabled(void)
{
	return m_instance->m_enabled;
}

void pBuzzer::setBeep(const bool isBeep)
{
	if (m_instance->m_enabled)
		m_instance->m_buzzer.SetBeep(isBeep);
}

void pBuzzer::setBeep(const uint8_t noteIndex, const uint16_t loudness)
{
	if (m_instance->m_enabled)
		m_instance->m_buzzer.Set((uint32_t)m_instance->notes[inRange(0, noteIndex, 87)].freq, inRange(0, loudness, 1000));
}

void pBuzzer::noteDown(const uint8_t noteIndex, const uint16_t loudness, const uint16_t delayMs, const uint16_t delayMsAfter)
{
	if (m_instance->m_enabled)
	{
		m_instance->setBeep(noteIndex, isDefault(loudness)? 333 : loudness);
		DelayMsByTicks(isDefault(delayMs)? 100 : delayMs);
		m_instance->setBeep(0, 0);
		DelayMsByTicks(isDefault(delayMsAfter)? 0 : delayMsAfter);
	}
}

void pBuzzer::startSong(void)
{
	m_instance->m_enabled = true;
	m_instance->noteDown(55, m_instance->defaultValue, m_instance->defaultValue, 50);
	m_instance->noteDown(55, m_instance->defaultValue, m_instance->defaultValue, 50);
	m_instance->noteDown(55, m_instance->defaultValue, m_instance->defaultValue, 200);
	m_instance->noteDown(51);
	m_instance->noteDown(55, m_instance->defaultValue, m_instance->defaultValue, 200);
	m_instance->noteDown(58, m_instance->defaultValue, m_instance->defaultValue, 350);
	m_instance->noteDown(46, m_instance->defaultValue, 200);
	m_instance->m_enabled = false;
}

void pBuzzer::endSong(void)
{
	m_instance->m_enabled = true;
	m_instance->noteDown(51);
	m_instance->noteDown(58, m_instance->defaultValue, m_instance->defaultValue, 200);
	m_instance->noteDown(58, m_instance->defaultValue, m_instance->defaultValue, 50);
	m_instance->noteDown(58, m_instance->defaultValue, m_instance->defaultValue, 100);
	m_instance->noteDown(56, m_instance->defaultValue, m_instance->defaultValue, 100);
	m_instance->noteDown(55, m_instance->defaultValue, m_instance->defaultValue, 100);
	m_instance->noteDown(53);
	m_instance->noteDown(44, m_instance->defaultValue, m_instance->defaultValue, 200);
	m_instance->noteDown(44);
	m_instance->noteDown(41, m_instance->defaultValue, 200);
	m_instance->m_enabled = false;
}

void pBuzzer::terminated(void)
{
	m_instance->m_enabled = true;
	pBuzzer::noteDown(51);
	m_instance->m_enabled = false;
}

const pBuzzer::note pBuzzer::notes[88] =
{
	{ "A0", 27.5 },			// 0
	{ "A#0", 29.1352 },
	{ "B0", 30.8677 },
	{ "C1", 32.7032 },
	{ "C#1", 34.6478 },
	{ "D1", 36.7081 },
	{ "D#1", 38.8909 },
	{ "E1", 41.2034 },
	{ "F1", 43.6535 },
	{ "F#1", 46.2493 },
	{ "G1", 48.9994 },		// 10
	{ "G#1", 51.9131 },
	{ "A1", 55.0000 },
	{ "A#1", 58.2705 },
	{ "B1", 61.7354 },
	{ "C2", 65.4064 },
	{ "C#2", 69.2957 },
	{ "D2", 73.4162 },
	{ "D#2", 77.7817 },
	{ "E2", 82.4069 },
	{ "F2", 87.3071 },		// 20
	{ "F#2", 92.4986 },
	{ "G2", 97.9989 },
	{ "G#2", 103.826 },
	{ "A2", 110.000 },
	{ "A#2", 116.541 },
	{ "B2", 123.471 },
	{ "C3", 130.813 },
	{ "C#3", 138.591},
	{ "D3", 146.832 },
	{ "D#3", 155.563 },		// 30
	{ "E3", 164.814 },
	{ "F3", 174.614 },
	{ "F#3", 184.997 },
	{ "G3", 195.998 },
	{ "G#3", 207.652 },
	{ "A3", 220.000 },
	{ "A#3", 233.082 },
	{ "B3", 246.942 },
	{ "C4", 261.626 },
	{ "C#4", 277.183 },		// 40
	{ "D4", 293.665 },
	{ "D#4", 311.127 },
	{ "E4", 329.628 },
	{ "F4", 349.228 },
	{ "F#4", 369.994 },
	{ "G4", 391.995 },
	{ "G#4", 415.305 },
	{ "A4", 440.000 },
	{ "A#4", 466.164 },
	{ "B4", 493.883 },		// 50
	{ "C5", 523.251 },
	{ "C#5", 554.365 },
	{ "D5", 587.330 },
	{ "D#5", 622.254 },
	{ "E5", 659.255 },
	{ "F5", 698.456 },
	{ "F#5", 739.989 },
	{ "G5", 783.991 },
	{ "G#5", 830.609 },
	{ "A5", 880.000 },		// 60
	{ "A#5", 932.328 },
	{ "B5", 987.767 },
	{ "C6", 1046.50 },
	{ "C#6", 1108.73 },
	{ "D6", 1174.66 },
	{ "D#6", 1244.51 },
	{ "E6", 1318.51 },
	{ "F6", 1396.91 },
	{ "F#6", 1479.98 },
	{ "G6", 1567.98 },		// 70
	{ "G#6", 1661.22 },
	{ "A6", 1760.00 },
	{ "A#6", 1864.66 },
	{ "B6", 1975.53 },
	{ "C7", 2093.00 },
	{ "C#7", 2217.46 },
	{ "D7", 2349.32 },
	{ "D#7", 2489.02 },
	{ "E7", 2637.02 },
	{ "F7", 2793.83 },		// 80
	{ "F#7", 2959.96 },
	{ "G7", 3135.96 },
	{ "G#7", 3322.44 },
	{ "A7", 3520.00 },
	{ "A#7", 3729.31 },
	{ "B7", 3951.07 },
	{ "C8", 4186.01 }		// 87
};
