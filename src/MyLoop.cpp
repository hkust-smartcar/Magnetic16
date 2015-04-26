/*
 * MyLoop.cpp
 *
 *  Created on: Feb 28, 2015
 *      Author: Peter
 */

#include <functional>
#include <vector>

#include <libsc/system.h>

#include "MyLoop.h"

using namespace std;

void DelayMsByTicks(Timer::TimerInt delay)
{
	Timer::TimerInt startTime = System::Time();

	while (Timer::TimeDiff(startTime, System::Time()) < delay);
}

MyLoop::MyLoop(void)
:
	m_start_time(0)
{
	System::Init();
}

void MyLoop::addFunctionToLoop(const LoopFunction &func, TimeInterval delay)
{
	if (m_timer_list.size() == 0)
		return ;

	for (uint8_t i = 0; i < m_timer_list.size(); i++)
		if (m_timer_list[i].interval > delay)
		{
			m_timer_list.emplace(m_timer_list.begin() + i, (TimerInfo) {func, delay, 0});
			return;
		}

	m_timer_list.emplace_back((TimerInfo) {func, delay, 0});
}

void MyLoop::start(void)
{
	m_start_time = System::Time();

	for (uint8_t i = 0; i < m_timer_list.size(); i++)
		m_timer_list[i].lastRunTime = m_start_time;

	while (true)
	{
		for (uint8_t i = 0; i < m_timer_list.size(); i++)
			if (System::Time() - m_timer_list[i].lastRunTime >= m_timer_list[i].interval)
			{
				((LoopFunction)m_timer_list[i].func)(System::Time() - m_timer_list[i].lastRunTime - m_timer_list[i].interval);
				m_timer_list[i].lastRunTime = System::Time();
			}
	}
}
