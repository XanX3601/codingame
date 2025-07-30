#ifndef SOAK_OVERFLOW_GRID_CELL_H
#define SOAK_OVERFLOW_GRID_CELL_H

#include <vector>

#include <soak_overflow/grid/CellType.h>
#include <soak_overflow/grid/Coord.h>

namespace soak_overflow::grid
{
    class Cell
    {
    // constructor *************************************************************
    public:
        Cell(Coord&& rv_coord);

        Cell(const Cell& cr_cell) = delete;

        Cell(Cell&& rv_cell) noexcept = default;

        ~Cell() noexcept = default;

    // coord *******************************************************************
    public:
        const Coord& get_coord() const;

    private:
        Coord __coord;

    // neighbors ***************************************************************
    public:
        void add_neighbor(const Cell* const p_cell, bool orthogonal = false);

        const std::vector<const Cell*>& get_neighbors(bool orthogonal = false) const;

    private:
        std::vector<const Cell*> __neighbors;

        std::vector<const Cell*> __orthogonal_neighbors;

    // operator ****************************************************************
    public:
        Cell& operator=(const Cell& cr_cell) = delete;

        Cell& operator=(Cell&& rv_cell) noexcept = delete;

    // type ********************************************************************
    public:
        const CellType& get_type() const;

        void set_type(CellType cell_type);

    private:
        CellType __type;
    };
}

#endif
