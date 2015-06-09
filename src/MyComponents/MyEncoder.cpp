/*
 * MyEncoder.cpp
 *
 * Author: Peter
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include <libsc/encoder.h>
#include <libsc/ab_encoder.h>
#include "MyEncoder.h"
#include "MyResource.h"

MyEncoder::MyEncoder(void)
:
	AbEncoder({ 0 }),
	m_lastCount(0)
{}

int32_t MyEncoder::getEncoderReading(void)
{
	Update();
	m_lastCount = GetCount();
	return m_lastCount;
}

int32_t *MyEncoder::getEncoderCountPointer(void)
{
	return &m_lastCount;
}

void MyEncoder::reset(void)
{
	Update();
	Update();
	m_lastCount = 0;
}
