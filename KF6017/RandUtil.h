#pragma once
#include <random>
#include <memory>

class RandUtil
{
public:
	static int randRangeInt(int min, int max);
	static double randRangeDouble(double min, double max);

#if !RELEASE
	static void Debug();
#endif

private:
	static std::mt19937 getGenerator();
};

