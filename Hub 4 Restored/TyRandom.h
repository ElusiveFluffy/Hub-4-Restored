#pragma once
namespace TyRandom
{
	inline int* RandomSeed = nullptr;

	float RandomFR(float min, float max);
	int RandomIR(int inclusiveMin, int exclusiveMax);
	void InitRandNumFunctions();
};
