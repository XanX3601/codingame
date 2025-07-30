#include <soak_overflow/grid/Cell.h>

namespace soak_overflow::grid
{
    // constructor *************************************************************
    Cell::Cell(Coord&& rv_coord):
        // coord
        __coord(std::move(rv_coord)),
        // neighbors
        __neighbors(),
        __orthogonal_neighbors(),
        // type
        __type(CellType::empty)
    {
        __neighbors.reserve(8);
        __orthogonal_neighbors.reserve(4);
    }

    // coord *******************************************************************
    const Coord& Cell::get_coord() const
    {
        return __coord;
    }

    // neighbors ***************************************************************
    void Cell::add_neighbor(const Cell* p_cell, bool orthogonal)
    {
        __neighbors.emplace_back(p_cell);
        if (orthogonal)
        {
            __orthogonal_neighbors.emplace_back(p_cell);
        }
    }

    const std::vector<const Cell*>& Cell::get_neighbors(bool orthogonal) const
    {
        if (orthogonal)
        {
            return __orthogonal_neighbors;
        }

        return __neighbors;
    }

    // type ********************************************************************
    const CellType& Cell::get_type() const
    {
        return __type;
    }

    void Cell::set_type(CellType cell_type)
    {
        __type = cell_type;
    }
}
