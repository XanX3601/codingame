#include <soak_overflow/grid/Grid.h>

#include <iostream>

namespace soak_overflow::grid
{
    // cells *******************************************************************
    const Cell& Grid::get_cell(const Coord& cr_coord) const
    {
        return __cells.at(__coord_to_cell_index(cr_coord));
    }

    int Grid::get_cell_count() const
    {
        return __cells.size();
    }

    std::vector<Cell>::size_type Grid::__coord_to_cell_index(const Coord& cr_coord) const
    {
        return cr_coord.y * __width + cr_coord.x;
    }

    // constructor *************************************************************
    Grid::Grid(
        int width,
        int height
    ):
        // cells
        __cells(),
        // size
        __height(height),
        __width(width)
    {
        for (int y(0); y < __height; ++y)
        {
            for (int x(0); x < __width; ++x)
            {
                __cells.emplace_back(Coord(x, y));
            }
        }

        for (int y(0); y < __height; ++y)
        {
            for (int x(0); x < __width; ++x)
            {
                Coord coord(x, y);
                Cell& r_cell(__cells.at(__coord_to_cell_index(coord)));

                for (const Direction& cr_direction: Direction::directions)
                {
                    Coord neighbor_coord(coord.apply_direction(cr_direction));
                    if (!is_in_grid(neighbor_coord))
                    {
                        continue;
                    }

                    Cell& r_neighbor(
                        __cells.at(__coord_to_cell_index(neighbor_coord))
                    );
                    r_cell.add_neighbor(
                        &r_neighbor,
                        cr_direction.is_orthogonal()
                    );
                }
            }
        }
    }

    // coord *******************************************************************
    bool Grid::is_in_grid(const Coord& cr_coord) const
    {
        return (
            0 <= cr_coord.x && cr_coord.x < __width
            && 0 <= cr_coord.y && cr_coord.y < __height
        );
    }

    // size ********************************************************************
    int Grid::get_height() const
    {
        return __height;
    }

    int Grid::get_width() const
    {
        return __width;
    }

    // update ******************************************************************
    void Grid::update_from_stdin()
    {
        int cell_count(get_cell_count());
        for (int cell_index(0); cell_index < cell_count; ++cell_index)
        {
            int x;
            int y;
            int cell_type;

            std::cin >> x >> y >> cell_type;
            std::cin.ignore();

            Cell& r_cell(__cells.at(__coord_to_cell_index({x, y})));
            r_cell.set_type(int_to_cell_type(cell_type));
        }
    }
}

