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

