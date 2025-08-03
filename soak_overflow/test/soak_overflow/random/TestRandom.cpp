#include <doctest/doctest.h>

#include <soak_overflow/random/Random.h>

TEST_CASE("soak_overflow::random::Random")
{
    SUBCASE("Given min 5 and max 42")
    {
        int min(5);
        int max(42);

        SUBCASE("When generating random integers between min and max")
        {
            for (int i(0); i < 1000; ++i)
            {
                int random(soak_overflow::random::Random::random_int(min, max));

                SUBCASE("Then they are all between min and max")
                {
                    CHECK(min <= random);
                    CHECK(random <= max);
                }
            }
        }
    }
}
