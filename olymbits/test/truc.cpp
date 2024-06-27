#include <catch2/catch_test_macros.hpp>

SCENARIO("Story of a truc") {
    GIVEN("truc equal to 5") {
        int truc = 5;

        THEN("truc equal 5") {
            REQUIRE(truc == 5);
        }
    }
}
