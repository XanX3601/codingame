#include <soak_overflow/grid/CellType.h>

namespace soak_overflow::grid
{
    CellType int_to_cell_type(int cell_type)
    {
        if (cell_type == 1)
        {
            return CellType::low_crate;
        }
        else if (cell_type == 2)
        {
            return CellType::high_crate;
        }

        return CellType::empty;
    }
}
