#include <soak_overflow/action/Move.h>

namespace soak_overflow::action
{
    // apply *******************************************************************
    grid::Coord Move::apply_to(const grid::Coord& cr_coord) const
    {
        return {
            cr_coord.x + __direction.delta_x,
            cr_coord.y + __direction.delta_y
        };
    }

    // constructor *************************************************************
    Move::Move(grid::Direction direction):
        // direction
        __direction(direction)
    {
    }

    // direction ***************************************************************
    const grid::Direction& Move::get_direction() const
    {
        return __direction;
    }

    // operator ****************************************************************
    bool Move::operator==(const Move& cr_move) const
    {
        return __direction == cr_move.__direction;
    }

    // moves *******************************************************************
    const Move Move::down({0, 1});

    const Move Move::left({-1, 0});

    const Move Move::right({1, 0});

    const Move Move::stay({0, 0});

    const Move Move::up({0, -1});

    const std::array<const Move, 5> Move::moves{
        down,
        left,
        right,
        stay,
        up
    };
}
