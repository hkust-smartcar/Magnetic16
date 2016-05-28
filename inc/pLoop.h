/*
 * pLoop.h
 *
 *  Created on: Feb 28, 2015
 *      Author: Peter
 */

#pragma once

#include <functional>
#include <vector>

#include <libsc/system.h>

using namespace libsc;
using namespace std;

#define LOOP_IMMEDIATELY	0
#define LOOP_EVERYTIME		1

#define maxValue(a, b) ((a > b)? a : b)

void DelayMsByTicks(Timer::TimerInt delay);

class pLoop
{
public:

	typedef function<void (const uint32_t &)> LoopFunction;
	typedef Timer::TimerInt TimeInterval;

	struct TimerInfo
	{
		LoopFunction	func;
		TimeInterval	interval;
		Timer::TimerInt	lastRunTime;
	};

	pLoop();

	void start(void);

	void addFunctionToLoop(const LoopFunction &func, TimeInterval delay);

private:

	vector<TimerInfo>			m_timer_list;

	Timer::TimerInt				m_start_time;

};
