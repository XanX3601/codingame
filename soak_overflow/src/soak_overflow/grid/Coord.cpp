#include <soak_overflow/grid/Coord.h>

#include <iostream>

namespace soak_overflow::grid
{
    // constructor *************************************************************
    Coord::Coord():
        // value
        x(0),
        y(0)
    {
    }
    
    Coord::Coord(int x, int y):
        x(x),
        y(y)
    {
    }

    // direction ***************************************************************
    Coord Coord::apply_direction(const Direction& cr_direction) const
    {
        return Coord(x + cr_direction.delta_x, y + cr_direction.delta_y);
    }

    // id **********************************************************************
    int Coord::get_id() const
    {
        return x * 1000 + y;
    }

    // operator ****************************************************************
    bool Coord::operator==(const Coord& cr_coord) const
    {
        return x == cr_coord.x && y == cr_coord.y;
    }

    bool Coord::operator!=(const Coord& cr_coord) const
    {
        return x != cr_coord.x || y != cr_coord.y;
    }

    std::ostream& operator<<(std::ostream& r_stream, const soak_overflow::grid::Coord& cr_coord)
    {
        r_stream
            << "("
            << cr_coord.x
            << ", "
            << cr_coord.y
            << ")";

        return r_stream;
    }
}



