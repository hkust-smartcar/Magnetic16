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

using namespace std;

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
	typedef RuleResult Rules[7][7];

	struct Config
	{
		MembershipFunc		errorMembershipFuncs;
		MembershipFunc		dErrorMembershipFuncs;
		MembershipFunc		outputMembershipFuncs;

		Rules					rules;

		float					approxAccuracy = 0.5f;
	};

	struct FuzzResult
	{
		uint8_t					relatedIndex[2];
		float					degreeOfMembship[2];
	};

	struct InferenceResult
	{
		bool outputIndice[7];
		float outputDom[7];
	};

	enum ErrorType
	{
		Error = 0,
		dError
	};

	explicit pFuzzyLogic(const Config &config);

	float update(float error, float dError);

	static size_t FuzzySetLen;
	static size_t MembShipFuncInputLen;

private:

	FuzzResult fuzzification(const float &val, const ErrorType &type);
	InferenceResult fuzzyInference(const FuzzResult &errorResult, const FuzzResult &dErrorResult);
	float defuzzification(const InferenceResult &inferResult);

	array<MembershipFunc, 2> m_MembershipFuncs;
	MembershipFunc			m_outputMembershipFuncs;

	Rules						m_rules;

	float						m_approxAccu;

};

