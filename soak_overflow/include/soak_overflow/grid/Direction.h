#ifndef SOAK_OVERFLOW_GRID_DIRECTION_H
#define SOAK_OVERFLOW_GRID_DIRECTION_H

#include <array>

namespace soak_overflow::grid
{
    class Direction
    {
    // constructor *************************************************************
    public:
        Direction(int delta_x, int delta_y);

        Direction(const Direction& cr_direction) = default;

        Direction(Direction&& rv_direction) noexcept = default;

        ~Direction() noexcept = default;

    // delta *******************************************************************
    public:
        int delta_x;

        int delta_y;
    
    // directions **************************************************************
    public:
        static const std::array<const Direction, 8> directions;

        static const Direction east;

        static const Direction none;

        static const Direction north;

        static const Direction north_east;

        static const Direction north_west;

        static const std::array<const Direction, 4> orthogonal_directions;

        static const Direction south;

        static const Direction south_east;

        static const Direction south_west;

        static const Direction west;

    // operator ****************************************************************
    public:
        Direction& operator=(const Direction& cr_direction) = default;

        Direction& operator=(Direction&& rv_direction) noexcept = default;

        bool operator==(const Direction& cr_direction) const;

    // orthogonal **************************************************************
    public:
        bool is_orthogonal() const;
    };
}

#endif
