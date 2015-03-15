/*
 * PIDhandler.cpp
 *
 *  Created on: Jan 15, 2015
 *      Author: Peter
 */

#include "PIDhandler.h"
#include <cstring>

using namespace libsc::k60;

PIDhandler::PIDhandler(float *ref, float kp, float ki, float kd)
:
	reference(ref),
	Kp(kp),
	Ki(ki),
	Kd(kd),
	eSum(0),
	lastError(0)
{
	System::Init();
}

PIDhandler::PIDhandler(float ref, float kp, float ki, float kd)
:
	reference(new float(ref)),
	Kp(kp),
	Ki(ki),
	Kd(kd),
	eSum(0),
	epsilon(10),
	output(0),
	v(10)
	lastError(0)
{
	System::Init();
}

void PIDhandler::setKp(float kp)
{
	Kp = MAX(0, kp);
}
void PIDhandler::setKi(float ki)
{
	Ki = MAX(0, ki);
}
void PIDhandler::setKd(float kd)
{
	Kd = MAX(0, kd);
}

float PIDhandler::getKp(void)
{
	return Kp;
}

float PIDhandler::getKi(void)
{
	return Ki;
}

float PIDhandler::getKd(void)
{
	return Kd;
}

void PIDhandler::reset()
{
	eSum = 0;
}

float PIDhandler::updatePID(float val, uint32_t dt)
{
	float error = *reference - val;
	float dE = (error - lastError) / dt;
	if (abs(error)>epsilon){
		eSum += error*dt;
	}
	if (abs(error)>=v){
		output = abs(Kp * error) + Ki * esum + Kd * de;
	}
	else{
		output = Kp * error + Ki * esum + Kd * de;
	}
	if (output>reference)
		output = reference;
	return (float)(output);

}
