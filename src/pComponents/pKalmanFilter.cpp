/*
 * ppKalmanFilter.h
 *
 * Edit:   Petel__
 * Author: Tommy Wong
 * Copyright (c) 2014-2016 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include <pKalmanFilter.h>

pKalmanFilter::pKalmanFilter(float &q, float &r, const float x,	const float p)
:
	m_q(&q), m_r(&r), m_k(0), m_x(x), m_lx(0), m_p(p), m_lp(0)
{}

float pKalmanFilter::filter(const float data)
{
	predictState();
	predictCovariance();
	gain();
	updateState(data);
	updateCovariance();
	return m_x;
}

void pKalmanFilter::predictState()
{
	m_lx = m_x;
}

void pKalmanFilter::predictCovariance()
{
	m_lp = m_p + *m_q;
}

void pKalmanFilter::updateState(const float z)
{
	m_x = m_lx + (m_k * (z - m_lx));
}

void pKalmanFilter::updateCovariance()
{
	m_p = (1 - m_k) * m_lp;
}

void pKalmanFilter::gain()
{
	m_k = m_lp / (m_lp + *m_r);
}
