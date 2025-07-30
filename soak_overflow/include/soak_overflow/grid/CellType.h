#ifndef SOAK_OVERFLOW_GRID_CELL_TYPE_H
#define SOAK_OVERFLOW_GRID_CELL_TYPE_H

namespace soak_overflow::grid
{
    enum CellType
    {
        empty,
        low_crate,
        high_crate
    };

    CellType int_to_cell_type(int cell_type);
}

#endif
