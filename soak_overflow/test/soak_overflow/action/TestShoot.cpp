#include <doctest/doctest.h>

#include <soak_overflow/action/Shoot.h>

TEST_CASE("soak_overflow::action::Shoot")
{
    SUBCASE("Given a shoot on target 3")
    {
        soak_overflow::action::Shoot shoot(3);

        SUBCASE("Then its target id is 3")
        {
            CHECK(shoot.get_target_id() == 3);
        }
    }
}
