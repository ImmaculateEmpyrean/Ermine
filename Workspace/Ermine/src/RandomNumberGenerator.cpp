#include "stdafx.h"
#include "RandomNumberGenerator.h"

namespace Ermine
{
#pragma region StaticDefinitions

    RandomNumber* RandomNumber::Rand = nullptr;
    std::once_flag RandomNumber::InitializationFlag;

#pragma endregion StaticDefinitions

    RandomNumber::RandomNumber()
        :
        gen(std::mt19937(rd()))
    {}


    int RandomNumber::GetNumberInRange(int RangeBegin, int RangeEnd)
    {
        //This Is An Extremely Important Assertion As The distr Constructor Will Crash If Range End Is Lesser Than Range Begin
        assert(RangeEnd > RangeBegin);

        std::uniform_int_distribution<> distr;
        distr = std::uniform_int_distribution<>(RangeBegin, RangeEnd - 1);

        return distr(RandomNumber::Get()->gen);
    }


    RandomNumber* RandomNumber::Get()
    {
        std::call_once(InitializationFlag, []() {
            Rand = new RandomNumber();
        });

        return Rand;
    }
}
