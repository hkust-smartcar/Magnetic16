/*
 * pLoop.cpp
 *
 * Author: Peter
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include <pLoop.h>

using namespace std;
using namespace libsc;

void DelayMsByTicks(Timer::TimerInt delay)
{
	Timer::TimerInt startTime = System::Time();

	while (System::Time() - startTime < delay);
}

pLoop::pLoop(void)
:
	m_start_time(0)
{
	System::Init();
}

void pLoop::addFunctionToLoop(const LoopFunction &func, TimeInterval interval)
{
	m_task_list.emplace_back((TaskInfo) {func, interval, 0});
}

void pLoop::start(void)
{
	if (m_task_list.size() == 0)
		return ;

	sort
	(
		m_task_list.begin(),
		m_task_list.end(),
		[] (TaskInfo a, TaskInfo b)
		{
			return (a.interval < b.interval);
		}
	);

	m_start_time = System::Time();

	for (uint8_t i = 0; i < m_task_list.size(); i++)
		m_task_list[i].lastRunTime = m_start_time;

	while (true)
	{
		for (uint8_t i = 0; i < m_task_list.size(); i++)
			if (System::Time() - m_task_list[i].lastRunTime >= m_task_list[i].interval)
			{
				((LoopFunction)m_task_list[i].func)();
				m_task_list[i].lastRunTime = System::Time();
			}
	}
}
