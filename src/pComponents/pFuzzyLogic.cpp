/*
 * pFuzzyLogic.cpp
 *
 * Author: Petel__
 * Copyright (c) 2014-2016 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include <pFuzzyLogic.h>

#define MAX(a, b) ((a > b)? a : b)
#define MIN(a, b) ((a < b)? a : b)

using namespace std;
using namespace libsc;

size_t pFuzzyLogic::FuzzySetLen = 7;
size_t pFuzzyLogic::MembShipFuncInputLen = 4;

pFuzzyLogic::pFuzzyLogic(const Config &config)
:
	m_MembershipFuncs{ 0 },
	m_outputMembershipFuncs{ 0 },
	m_rules{ 0 },
	m_approxAccu(config.approxAccuracy),
	m_lastOutput(0.0f),
	m_lastError(0.0f),
	m_lastDError(0.0f),
	m_lastTime(0)
{
	memcpy(m_MembershipFuncs[ErrorType::Error], config.errorMembershipFuncs, 7 * 4 * sizeof(float));
	memcpy(m_MembershipFuncs[ErrorType::dError], config.dErrorMembershipFuncs, 7 * 4 * sizeof(float));
	memcpy(m_outputMembershipFuncs, config.outputMembershipFuncs, 7 * 4 * sizeof(float));
	memcpy(m_rules, config.rules, 7 * 7 * sizeof(uint8_t));
}

void pFuzzyLogic::resetPdController(void)
{
	m_lastTime = 0;
	m_lastError = 0.0f;
	m_lastOutput = 0.0f;
}

float pFuzzyLogic::updatePdController(float error)
{
	if (m_lastTime)
	{
		FuzzResult errorRet = fuzzification(error, ErrorType::Error);
		FuzzResult dErrorRet = fuzzification((m_lastDError = (error - m_lastError) * 1000.0f / (float)(m_dt = System::Time() - m_lastTime)), ErrorType::dError);

		InferenceResult inferResult = fuzzyInference(errorRet, dErrorRet);

		m_lastError = error;

		m_lastOutput = defuzzification(inferResult);
	}

	m_lastTime = System::Time();
	return m_lastOutput;
}

float pFuzzyLogic::update(float error, float dError)
{
	FuzzResult errorRet = fuzzification(error, ErrorType::Error);
	FuzzResult dErrorRet = fuzzification(dError, ErrorType::dError);

	InferenceResult inferResult = fuzzyInference(errorRet, dErrorRet);

	return defuzzification(inferResult);
}

float &pFuzzyLogic::getOutput(void)
{
	return m_lastOutput;
}

float &pFuzzyLogic::getDError(void)
{
	return m_lastDError;
}

pFuzzyLogic::FuzzResult pFuzzyLogic::fuzzification(const float &val, const ErrorType &type)
{
	float tempDegreeOfMembship = 0.0f;
	FuzzResult ret;

	for (uint8_t i = 0; i < 7; i++)
	{
		if ((val > m_MembershipFuncs[type][i][0] && val < m_MembershipFuncs[type][i][3]) ||
			(val == m_MembershipFuncs[type][i][0] && m_MembershipFuncs[type][i][0] == m_MembershipFuncs[type][i][1]) ||
			(val == m_MembershipFuncs[type][i][2] && m_MembershipFuncs[type][i][2] == m_MembershipFuncs[type][i][3]))
		{
			if (val >= m_MembershipFuncs[type][i][1] && val <= m_MembershipFuncs[type][i][2])
				tempDegreeOfMembship = 1.0f;
			else if (val < m_MembershipFuncs[type][i][1])
				tempDegreeOfMembship = (val - m_MembershipFuncs[type][i][0]) / (m_MembershipFuncs[type][i][1] - m_MembershipFuncs[type][i][0]);
			else
				tempDegreeOfMembship = (m_MembershipFuncs[type][i][3] - val) / (m_MembershipFuncs[type][i][3] - m_MembershipFuncs[type][i][2]);

			if (ret.relatedIndex[0] >= 7)
			{
				ret.relatedIndex[0] = i;
				ret.degreeOfMembship[0] = tempDegreeOfMembship;
			}
			else
			{
				ret.relatedIndex[1] = i;
				ret.degreeOfMembship[1] = tempDegreeOfMembship;
				break;
			}
		}
	}

	return ret;
}

pFuzzyLogic::InferenceResult pFuzzyLogic::fuzzyInference(const FuzzResult &errorResult, const FuzzResult &dErrorResult)
{
	InferenceResult ret;

	for (uint8_t i = 0, index = 0; i < 2; i++)
	{
		if (dErrorResult.relatedIndex[i] < 7)
		{
			for (uint8_t j = 0; j < 2; j++)
			{
				if (errorResult.relatedIndex[j] < 7)
				{
					index = m_rules[dErrorResult.relatedIndex[i]][errorResult.relatedIndex[j]];
					if (ret.outputIndice[index])
						ret.outputDom[index] = MAX(ret.outputDom[index], MIN(errorResult.degreeOfMembship[j], dErrorResult.degreeOfMembship[i]));
					else
						ret.outputDom[index] = MIN(errorResult.degreeOfMembship[j], dErrorResult.degreeOfMembship[i]);
					ret.outputIndice[index] = true;
				}
			}
		}
	}

	return ret;
}

float pFuzzyLogic::defuzzification(const InferenceResult &inferResult)
{
	uint8_t outputIndex = 0, start = 7, end = 0, cur = 7;
	float moment = 0.0f, mass = 0.0f, tempMass = 0.0f;

	for (uint8_t i = 0; i < 7; i++)
		if (inferResult.outputIndice[i])
		{
			if (start == 7)
				start = i;
			end = i;
		}
	cur = start;

	for (float x = m_outputMembershipFuncs[cur][0]; x <= m_outputMembershipFuncs[end][3]; x += m_approxAccu)
	{
		if (x > m_outputMembershipFuncs[cur][3])
			cur++;

		if (m_outputMembershipFuncs[cur][1] - m_outputMembershipFuncs[cur][0] == 0.0f)
			tempMass = 1;
		else if (x < m_outputMembershipFuncs[cur][1])
			tempMass = MIN(inferResult.outputDom[cur], (x - m_outputMembershipFuncs[cur][0]) / (m_outputMembershipFuncs[cur][1] - m_outputMembershipFuncs[cur][0]));
		else if (x < m_outputMembershipFuncs[cur][2])
			tempMass = inferResult.outputDom[cur];
		else if (m_outputMembershipFuncs[cur][3] - m_outputMembershipFuncs[cur][2] == 0.0f)
			tempMass = 1;
		else if (cur + 1 >= 7 || x <= m_outputMembershipFuncs[cur + 1][0])
			tempMass = MIN(inferResult.outputDom[cur], (m_outputMembershipFuncs[cur][3] - x) / (m_outputMembershipFuncs[cur][3] - m_outputMembershipFuncs[cur][2]));
		else if (x > m_outputMembershipFuncs[cur + 1][0])
			tempMass = MAX(MIN(inferResult.outputDom[cur], (m_outputMembershipFuncs[cur][3] - x) / (m_outputMembershipFuncs[cur][3] - m_outputMembershipFuncs[cur][2])), MIN(inferResult.outputDom[cur + 1], (x - m_outputMembershipFuncs[cur + 1][0]) / (m_outputMembershipFuncs[cur + 1][1] - m_outputMembershipFuncs[cur + 1][0])));

		moment += tempMass * x;
		mass += tempMass;
	}

	return moment / mass;
}
