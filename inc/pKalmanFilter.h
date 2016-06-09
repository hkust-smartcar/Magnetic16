/*
 * pKalmanFilter.h
 *
 * Edit:   Petel__
 * Author: Tommy Wong
 * Copyright (c) 2014-2016 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

class pKalmanFilter
{

public:

	pKalmanFilter(float &q, float &r, const float x, const float p);

	float filter(const float data);

private:

	void predictState();
	void predictCovariance();
	void updateState(const float z);
	void updateCovariance();
	void gain();

protected:

	float *m_q, *m_r, m_k, m_x, m_lx, m_p, m_lp;

};
