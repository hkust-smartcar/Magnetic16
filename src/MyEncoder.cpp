/*
 * MyEncoder.cpp
 *
 * Author: Peter
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */


#include <libsc/ab_encoder.h>
#include "MyEncoder.h"
#include "MyResource.h"

MyEncoder::MyEncoder(void)
:
	AbEncoder({ 0 }),
	lastCount(0)
{}
