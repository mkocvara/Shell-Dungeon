#include "RandUtil.h"

#if _DEBUG
#include "ErrorLogger.h"
#endif

// PUBLIC

int RandUtil::randRangeInt(int min, int max)
{
    static std::mt19937 sGen = getGenerator();
    std::uniform_int_distribution<> distr(min, max); // define the range
    return distr(sGen);
}

double RandUtil::randRangeDouble(double min, double max)
{
    static std::mt19937 sGen = getGenerator();
    std::uniform_real_distribution<> distr(min, max); // define the range
    return distr(sGen);
}

#if _DEBUG
void RandUtil::Debug()
{
    for (int i = 0; i < 20; i++)
    {
        ErrorLogger::Write(L"Random between 1 and 20: ");
        ErrorLogger::Write(randRangeInt(1, 20));
        ErrorLogger::Write(L"\n");
    }
}
#endif

// PRIVATE

std::mt19937 RandUtil::getGenerator()
{
    static std::random_device sRD;
    static std::mt19937 sGen(sRD());
    return sGen;
}
