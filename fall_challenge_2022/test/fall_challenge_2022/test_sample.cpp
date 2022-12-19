#include <catch2/catch_test_macros.hpp>

SCENARIO("Testing catch2")
{
    GIVEN("Two numbers 1 and 2")
    {
        int a(1);
        int b(2);

        WHEN("Adding 1 and 2")
        {
            int c(a + b);

            THEN("The result is 3")
            {
                REQUIRE(c == 3);
            }
        }
    }
}
