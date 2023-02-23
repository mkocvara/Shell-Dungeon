#pragma once
#include <random>
#include <memory>

static class RandUtil
{
public:
	static int randRangeInt(int min, int max);
	static float randRangeFloat(float min, float max);

#if !RELEASE
	static void debug();
#endif

private:
	static std::mt19937 getGenerator();
};

