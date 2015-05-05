/*
 * MyKalmanFilter.cpp
 *
 * Author: Tommy Wong
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include <cassert>
#include <cstddef>

#include "MyKalmanFilter.h"

MyKalmanFilter::MyKalmanFilter(float &q, float &r, const float x, const float p)
:
	m_isUsable(true),
	m_q(q), m_r(r), m_k(0), m_x(x), m_lx(0), m_p(p), m_lp(0)
{}

float MyKalmanFilter::Filter(const float data)
{
	if (!m_isUsable) assert(false);
	PredictState();
	PredictCovariance();
	Gain();
	UpdateState(data);
	UpdateCovariance();
	return m_x;
}

void MyKalmanFilter::PredictState()
{
	m_lx = m_x;
}

void MyKalmanFilter::PredictCovariance()
{
	m_lp = m_p + m_q;
}

void MyKalmanFilter::UpdateState(const float z)
{
	m_x = m_lx + (m_k * (z - m_lx));
}

void MyKalmanFilter::UpdateCovariance()
{
	m_p = (1 - m_k) * m_lp;
}

void MyKalmanFilter::Gain()
{
	m_k = m_lp / (m_lp + m_r);
}
