#include <soak_overflow/action/Combat.h>

namespace soak_overflow::action
{
    // constructor *************************************************************
    Combat::Combat(
        grid::Coord coord,
        bool is_thrown
    ):
        __coord(coord),
        __throw(is_thrown)
    {
    }

    // coord *******************************************************************
    const grid::Coord& Combat::get_coord() const
    {
        return __coord;
    }

    // throw *******************************************************************
    bool Combat::is_thrown() const
    {
        return __throw;
    }
}
