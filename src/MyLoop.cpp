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

void MyLoop::addFunctionToLoop(const LoopFunction &func, Timer::TimerInt delay, uint16_t often)
{
	m_function_list.push_back(func);
	m_delay_list.push_back(delay);
	m_often_list.push_back(often);
	m_counter_list.push_back(0);
}

void MyLoop::start(void)
{
	Timer::TimerInt timeDiff = 0;

	while (true)
	{
		for (uint16_t i = 0; i < m_function_list.size(); i++)
		{
			if (m_counter_list[i] >= m_often_list[i])
			{
				((LoopFunction)m_function_list[i])();
				m_counter_list[i] = 0;
			}
			DelayMsByTicks(m_delay_list[i]);
			(m_counter_list[i])++;
		}
	}
}
