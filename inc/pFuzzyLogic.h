/*
 * pFuzzyLogic.h
 *
 * Author: Petel__
 * Copyright (c) 2014-2016 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <array>
#include <cstring>
#include <libsc/system.h>

typedef unsigned char uint8_t;

using namespace std;
using namespace libsc;

class pFuzzyLogic
{

public:

	/**
	 * Using miniMax FuzzySet Operation,
	 * CenterOfArea defuzzification,
	 * NL NM NS ZR PS PM PL
	 */

	typedef float MembShipFuncInput[4];
	// Len: membershipFuncLen
	typedef MembShipFuncInput MembershipFunc[7];
	// index of element in output fuzzy set
	typedef uint8_t RuleResult;
	// Rules[dError][error]
	/**
	 *				  error
	 * 		   NL NM NS ZR PS PM PL
	 * 		NL
	 * 	d	NM
	 * 	E	NS
	 * 	r	ZR
	 * 	r	PS
	 * 	o	PM
	 * 	r	PL
	 *
	 */
	typedef RuleResult Rules[7][7];

	struct Config
	{
		MembershipFunc		errorMembershipFuncs;
		MembershipFunc		dErrorMembershipFuncs;
		MembershipFunc		outputMembershipFuncs;

		Rules				rules;

		float				approxAccuracy = 1.0f;
	};

	struct FuzzResult
	{
		uint8_t				relatedIndex[2];
		float				degreeOfMembship[2];

		FuzzResult(void)
		:
			relatedIndex{ 7, 7 },
			degreeOfMembship{ 0.0f }
		{}
	};

	struct InferenceResult
	{
		bool				outputIndice[7];
		float				outputDom[7];

		InferenceResult(void)
		:
			outputIndice{ false },
			outputDom{ 0.0f }
		{}
	};

	enum ErrorType
	{
		Error = 0,
		dError
	};

	explicit pFuzzyLogic(const Config &config);

	void resetPdController(void);

	float updatePdController(float error);
	float update(float error, float dError);

	float &getOutput(void);
	float &getDError(void);

	static size_t FuzzySetLen;
	static size_t MembShipFuncInputLen;

private:

	FuzzResult fuzzification(const float &val, const ErrorType &type);
	InferenceResult fuzzyInference(const FuzzResult &errorResult, const FuzzResult &dErrorResult);
	float defuzzification(const InferenceResult &inferResult);

	array<MembershipFunc, 2> 	m_MembershipFuncs;
	MembershipFunc				m_outputMembershipFuncs;

	Rules						m_rules;

	float						m_approxAccu;

	Timer::TimerInt				m_dt;
	float						m_lastOutput;
	float						m_lastError;
	float						m_lastDError;

	Timer::TimerInt				m_lastTime;

};

