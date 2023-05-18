/*
* RandUtil.h
* 19002175
*
* Static utility class which uses the mt19937 random generator for a uniformly distributed range number generation.
*/

#pragma once
#include <random>
#include <memory>
#include "vector2D.h"

class RandUtil
{
public:
	static int randRangeInt(int min, int max);
	static double randRangeDouble(double min, double max);

	static Vector2D randDirectionVector();

#if !RELEASE
	static void Debug();
#endif

private:
	static std::mt19937 getGenerator();
};

