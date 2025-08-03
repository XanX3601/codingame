#include <doctest/doctest.h>

#include <soak_overflow/action/Move.h>

TEST_CASE("soak_overflow::action::Move")
{
    SUBCASE("down direction is south")
    {
        CHECK(soak_overflow::action::Move::down.get_direction() == soak_overflow::grid::Direction::south);
    }
    SUBCASE("left direction is west")
    {
        CHECK(soak_overflow::action::Move::left.get_direction() == soak_overflow::grid::Direction::west);
    }
    SUBCASE("right direction is east")
    {
        CHECK(soak_overflow::action::Move::right.get_direction() == soak_overflow::grid::Direction::east);
    }
    SUBCASE("stay direction is none")
    {
        CHECK(soak_overflow::action::Move::stay.get_direction() == soak_overflow::grid::Direction::none);
    }
    SUBCASE("up direction is north")
    {
        CHECK(soak_overflow::action::Move::up.get_direction() == soak_overflow::grid::Direction::north);
    }

    SUBCASE("Given a coord (2, 5)")
    {
        soak_overflow::grid::Coord coord(2, 5);

        SUBCASE("When applying down move")
        {
            soak_overflow::grid::Coord new_coord(
                soak_overflow::action::Move::down.apply_to(coord)
            );

            SUBCASE("Then the new coord is (2, 6)")
            {
                CHECK(new_coord == soak_overflow::grid::Coord(2, 6));
            }
        }
        SUBCASE("When applying left move")
        {
            soak_overflow::grid::Coord new_coord(
                soak_overflow::action::Move::left.apply_to(coord)
            );

            SUBCASE("Then the new coord is (1, 5)")
            {
                CHECK(new_coord == soak_overflow::grid::Coord(1, 5));
            }
        }
        SUBCASE("When applying right move")
        {
            soak_overflow::grid::Coord new_coord(
                soak_overflow::action::Move::right.apply_to(coord)
            );

            SUBCASE("Then the new coord is (3, 5)")
            {
                CHECK(new_coord == soak_overflow::grid::Coord(3, 5));
            }
        }
        SUBCASE("When applying stay move")
        {
            soak_overflow::grid::Coord new_coord(
                soak_overflow::action::Move::stay.apply_to(coord)
            );

            SUBCASE("Then the new coord is (2, 5)")
            {
                CHECK(new_coord == soak_overflow::grid::Coord(2, 5));
            }
        }
        SUBCASE("When applying up move")
        {
            soak_overflow::grid::Coord new_coord(
                soak_overflow::action::Move::up.apply_to(coord)
            );

            SUBCASE("Then the new coord is (2, 4)")
            {
                CHECK(new_coord == soak_overflow::grid::Coord(2, 4));
            }
        }
    }

    SUBCASE("all moves are in the moves array")
    {
        bool down_found(false);
        bool left_found(false);
        bool right_found(false);
        bool stay_found(false);
        bool up_found(false);

        for (
            const soak_overflow::action::Move& cr_move
            : soak_overflow::action::Move::moves
        )
        {
            if (cr_move == soak_overflow::action::Move::down)
            {
                down_found = true;
            }
            else if (cr_move == soak_overflow::action::Move::left)
            {
                left_found = true;
            }
            else if (cr_move == soak_overflow::action::Move::right)
            {
                right_found = true;
            }
            else if (cr_move == soak_overflow::action::Move::stay)
            {
                stay_found = true;
            }
            else if (cr_move == soak_overflow::action::Move::up)
            {
                up_found = true;
            }
        }

        CHECK(down_found);
        CHECK(left_found);
        CHECK(right_found);
        CHECK(stay_found);
        CHECK(up_found);
    }
}
