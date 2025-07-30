#ifndef SOAK_OVERFLOW_GRID_GRID_H
#define SOAK_OVERFLOW_GRID_GRID_H

#include <vector>

#include <soak_overflow/grid/Cell.h>

namespace soak_overflow::grid
{
    class Grid
    {
    // cells *******************************************************************
    public:
        const Cell& get_cell(const Coord& cr_coord) const;

        int get_cell_count() const;

    private:
        std::vector<Cell>::size_type __coord_to_cell_index(const Coord& cr_coord) const;

        std::vector<Cell> __cells;

    // constructor *************************************************************
    public:
        Grid(
            int width,
            int height
        );

        Grid(const Grid& cr_grid) = delete;

        Grid(Grid&& rv_grid) noexcept = delete;

        ~Grid() noexcept = default;

    // coord *******************************************************************
    public:
        bool is_in_grid(const Coord& cr_coord) const;

    // operator ****************************************************************
    public:
        Grid& operator=(const Grid& cr_grid) = delete;

        Grid& operator=(Grid&& rv_grid) = delete;

    // size ********************************************************************
    public:
        int get_height() const;

        int get_width() const;

    private:
        int __height;

        int __width;

    // update ******************************************************************
    public:
        void update_from_stdin();
    };
}

#endif
