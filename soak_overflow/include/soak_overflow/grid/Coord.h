#ifndef SOAK_OVERFLOW_GRID_COORD_H
#define SOAK_OVERFLOW_GRID_COORD_H

#include <soak_overflow/grid/Direction.h>

namespace soak_overflow::grid
{
    class Coord
    {
    // constructor *************************************************************
    public:
        Coord();

        Coord(int x, int y);

        Coord(const Coord& cr_coord) = default;

        Coord(Coord&& rv_coord) noexcept = default;

        ~Coord() noexcept = default;

    // direction ***************************************************************
    public:
        Coord apply_direction(const Direction& cr_direction) const;

    // id **********************************************************************
    public:
        int get_id() const;

    // operator ****************************************************************
    public:
        Coord& operator=(const Coord& cr_coord) = default;

        Coord& operator=(Coord&& rv_coord) noexcept = default;

        bool operator==(const Coord& cr_coord) const;

        bool operator!=(const Coord& cr_coord) const;

        friend std::ostream& operator<<(
            std::ostream& r_stream,
            const Coord& cr_coord
        );

    // value *******************************************************************
    public:
        int x;

        int y;
    };
}

#endif
