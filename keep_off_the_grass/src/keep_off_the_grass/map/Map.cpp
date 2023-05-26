#include <keep_off_the_grass/map/Map.h>

namespace keep_off_the_grass::map
{
    // cell ********************************************************************
    const Cell& Map::get_cell(int x, int y) const
    {
        return __cells.at(__coordinates_to_cell_index(x, y));
    }

    Cell& Map::get_cell(int x, int y)
    {
        return __cells.at(__coordinates_to_cell_index(x, y));
    }

    int Map::__coordinates_to_cell_index(
        int x,
        int y
    ) const
    {
        return y * __width + x;
    }

    // constructor *************************************************************
    Map::Map(
        int width,
        int height
    ):
        // cell
        __cells(),
        // size
        __height(height),
        __width(width)
    {
        // create cells
        __cells.reserve(get_cell_count());

        for (int y(0); y < __height; ++y)
        {
            for (int x(0); x < __width; ++x)
            {
                __cells.emplace_back(x, y);
            }
        }

        // configure neighbors
        for (int x(0); x < __width; ++x)
        {
            for (int y(0); y < __height; ++y)
            {
                Cell& r_cell(get_cell(x, y));

                // east
                if (x + 1 < __width)
                {
                    r_cell.add_neighbor(get_cell(x + 1, y), Direction::east);
                }

                // north
                if (y - 1 >= 0)
                {
                    r_cell.add_neighbor(get_cell(x, y - 1), Direction::north);
                }

                // south
                if (y + 1 < __height)
                {
                    r_cell.add_neighbor(get_cell(x, y + 1), Direction::south);
                }

                // west
                if (x - 1 >= 0)
                {
                    r_cell.add_neighbor(get_cell(x - 1, y), Direction::west);
                }

            }
        }
    }

    // size ********************************************************************
    int Map::get_cell_count() const
    {
        return __width * __height;
    }

    int Map::get_height() const
    {
        return __height;
    }

    int Map::get_width() const
    {
        return __width;
    }
}
