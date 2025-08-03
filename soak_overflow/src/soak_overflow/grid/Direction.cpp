#include <soak_overflow/grid/Direction.h>

namespace soak_overflow::grid
{
    // constructor *************************************************************
    Direction::Direction(
        int delta_x,
        int delta_y
    ):
        // delta
        delta_x(delta_x),
        delta_y(delta_y)
    {
    }

    // directions **************************************************************
    const Direction Direction::east(1, 0);

    const Direction Direction::none(0, 0);

    const Direction Direction::north(0, -1);

    const Direction Direction::north_east(1, -1);

    const Direction Direction::north_west(-1, -1);

    const Direction Direction::south(0, 1);

    const Direction Direction::south_east(1, 1);

    const Direction Direction::south_west(-1, 1);

    const Direction Direction::west(-1, 0);

    // must be define after all directions have been instantiated
    const std::array<const Direction, 8> Direction::directions{
        east,
        north,
        north_east,
        north_west,
        south,
        south_east,
        south_west,
        west
    };

    const std::array<const Direction, 4> Direction::orthogonal_directions{
        east,
        north,
        south,
        west
    };

    // operator ****************************************************************
    bool Direction::operator==(const Direction& cr_direction) const
    {
        return (
            delta_x == cr_direction.delta_x
            && delta_y == cr_direction.delta_y
        );
    }

    // orthogonal **************************************************************
    bool Direction::is_orthogonal() const
    {
        return (
            (delta_x == 0 && delta_y != 0)
            || (delta_x != 0 && delta_y == 0)
        );
    }

}
