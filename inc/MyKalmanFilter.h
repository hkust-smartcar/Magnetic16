/*
 * MyKalmanFilter.h
 *
 * Author: Tommy Wong
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include <cstddef>

#pragma once

using namespace std;

class MyKalmanFilter
{

public:

	MyKalmanFilter(float *q, float *r, const float x, const float p);
	MyKalmanFilter(nullptr_t);

	float Filter(const float data);

private:

	void PredictState();
	void PredictCovariance();
	void UpdateState(const float z);
	void UpdateCovariance();
	void Gain();

	bool m_isUsable;

	float *m_q, *m_r, m_k, m_x, m_lx, m_p, m_lp;
};
