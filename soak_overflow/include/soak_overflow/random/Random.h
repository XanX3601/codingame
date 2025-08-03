#ifndef SOAK_OVERFLOW_RANDOM_RANDOM_H
#define SOAK_OVERFLOW_RANDOM_RANDOM_H

#include <random>

namespace soak_overflow::random
{
    class Random
    {
    // constructor *************************************************************
    public:
        ~Random() noexcept = default;

    private:
        Random();

        Random(const Random& cr_random) = delete;
        
        Random(Random&& rv_random) noexcept = delete;

    // generator ***************************************************************
    public:
        static int random_int(int min, int max);

    private:
        static std::ranlux24 __generator;

    // operator ****************************************************************
    private:
        Random& operator=(const Random& cr_random) = delete;

        Random& operator=(Random&& rv_random) noexcept = delete;
    };
}

#endif
