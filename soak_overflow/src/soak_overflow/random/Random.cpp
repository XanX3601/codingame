#include <soak_overflow/random/Random.h>

namespace soak_overflow::random
{
    // constructor *************************************************************
    Random::Random()
    {
    }

    // generator ***************************************************************
    int Random::random_int(int min, int max)
    {
        return (std::uniform_int_distribution<int>(min, max))(__generator);
    }

    std::ranlux24 Random::__generator{
        std::random_device()()
    };
}
