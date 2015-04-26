/*
 * MyMotor.h
 *
 *  Created on: Apr 25, 2015
 *      Author: Peter
 */

#pragma once

#include <libsc/dir_motor.h>
#include <libsc/ab_encoder.h>
#include "MyResource.h"

using namespace libsc;

class MyMotor : DirMotor
{

public:

	class MyEncoder : AbEncoder
	{

	public:

		MyEncoder(void);

	private:

	};
	MyEncoder	m_encoder;

	MyMotor(void);

private:

};
