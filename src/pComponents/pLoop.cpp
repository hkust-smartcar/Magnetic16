/*
 * pLoop.cpp
 *
 * Author: Peter
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include <functional>
#include <vector>

#include <libsc/system.h>

#include <pLoop.h>

using namespace std;
using namespace libsc;

void DelayMsByTicks(Timer::TimerInt delay)
{
	Timer::TimerInt startTime = System::Time();

	while (System::Time() - startTime < delay);
}

pLoop::pLoop(void)
{
	System::Init();
}

void pLoop::addFunctionToLoop(const LoopFunction &func, TimeInterval delay)
{
	m_task_list.emplace_back((TaskInfo) {func, delay});
}

void pLoop::start(void)
{
	if (m_task_list.size() == 0)
		return ;

//	sort
//	(
//		m_task_list.begin(),
//		m_task_list.end(),
//		[] (TaskInfo a, TaskInfo b)
//		{
//			return (a.interval < b.interval);
//		}
//	);

	Timer::TimerInt lastTime = System::Time();

//	TimeInterval prevTime = 0;
//	for (uint8_t i = 0; i < m_task_list.size(); i++)
//	{
//		m_task_list[i].interval -= prevTime;
//		prevTime = m_task_list[i].interval;
//	}

	while (true)
	{
		for (uint8_t i = 0; i < m_task_list.size(); i++)
		{
			assert(lastTime <= System::Time());
//			while (System::Time() - lastTime < m_task_list[i].interval);
			while (System::Time() - lastTime < m_task_list[i].delay);
			lastTime = System::Time();
			((LoopFunction)m_task_list[i].func)();
		}
	}
}
