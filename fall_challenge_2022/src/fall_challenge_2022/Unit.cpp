#include <fall_challenge_2022/Unit.h>

namespace fall_challenge_2022
{
    // constructor *************************************************************
    Unit::Unit(
        int x,
        int y,
        Player owner
    ):
        // coordinates
        __x(x),
        __y(y),
        // owner
        __owner(owner)
    {
    }

    // coordinates *************************************************************
    int Unit::get_x() const
    {
        return __x;
    }

    int Unit::get_y() const
    {
        return __y;
    }

    // owner *******************************************************************
    Player Unit::get_owner() const
    {
        return __owner;
    }
}
