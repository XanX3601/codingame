#include <soak_overflow/action/Throw.h>

namespace soak_overflow::action
{
    // constructor *************************************************************
    Throw::Throw(const grid::Coord& cr_coord):
        // coord
        __coord(cr_coord)
    {
    }

    // coord *******************************************************************
    const grid::Coord& Throw::get_coord() const
    {
        return __coord;
    }
}
