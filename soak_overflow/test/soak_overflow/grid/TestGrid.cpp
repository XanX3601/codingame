#include <doctest/doctest.h>

#include <sstream>
#include <iostream>

#include <soak_overflow/grid/Grid.h>

TEST_CASE("soak_overflow::grid::Grid")
{
    SUBCASE("Given a 3x4 grid")
    {
        soak_overflow::grid::Grid grid(3, 4);

        SUBCASE("Then its height is 4 and width is 3")
        {
            CHECK(grid.get_height() == 4);
            CHECK(grid.get_width() == 3);
        }

        SUBCASE("Then the cell (0, 0) has 3 neighors, 2 are orthogonals")
        {
            soak_overflow::grid::Coord coord;
            const soak_overflow::grid::Cell& cr_cell(grid.get_cell(coord));

            CHECK(cr_cell.get_neighbors().size() == 3);
            CHECK(cr_cell.get_neighbors(true).size() == 2);

            bool one_zero_found(false);
            bool zero_one_found(false);
            bool one_one_found(false);
            for (const soak_overflow::grid::Cell* cp_neighbor: cr_cell.get_neighbors())
            {
                if (cp_neighbor->get_coord() == soak_overflow::grid::Coord(1, 0))
                {
                    one_zero_found = true;
                }
                else if (cp_neighbor->get_coord() == soak_overflow::grid::Coord(0, 1))
                {
                    zero_one_found = true;
                }
                else if (cp_neighbor->get_coord() == soak_overflow::grid::Coord(1, 1))
                {
                    one_one_found = true;
                }
            }

            CHECK(one_zero_found);
            CHECK(zero_one_found);
            CHECK(one_one_found);

            one_zero_found = false;
            zero_one_found = false;
            one_one_found = false;

            for (const soak_overflow::grid::Cell* cp_neighbor: cr_cell.get_neighbors(true))
            {
                if (cp_neighbor->get_coord() == soak_overflow::grid::Coord(1, 0))
                {
                    one_zero_found = true;
                }
                else if (cp_neighbor->get_coord() == soak_overflow::grid::Coord(0, 1))
                {
                    zero_one_found = true;
                }
                else if (cp_neighbor->get_coord() == soak_overflow::grid::Coord(1, 1))
                {
                    one_one_found = true;
                }
            }

            CHECK(one_zero_found);
            CHECK(zero_one_found);
            CHECK(!one_one_found);
        }

        SUBCASE("Then the cell (2, 1) has 5 neighbors, 3 are orthogonals")
        {
            soak_overflow::grid::Coord coord(2, 1);
            const soak_overflow::grid::Cell& cr_cell(grid.get_cell(coord));

            CHECK(cr_cell.get_neighbors().size() == 5);
            CHECK(cr_cell.get_neighbors(true).size() == 3);
        }

        SUBCASE("When updating it through stdin")
        {
            std::string input(
                "0 0 0 1 0 1 2 0 2\n"
                "0 1 1 2 1 0 2 1 1\n"
                "0 2 2 1 2 1 2 2 0\n"
                "0 3 1 1 3 2 2 3 1"
            );
            std::istringstream input_stream(input);
            std::cin.rdbuf(input_stream.rdbuf());

            grid.update_from_stdin();

            SUBCASE("Then the cell's types are what is expected")
            {
                CHECK(grid.get_cell({0, 0}).get_type() == soak_overflow::grid::CellType::empty);
                CHECK(grid.get_cell({1, 0}).get_type() == soak_overflow::grid::CellType::low_crate);
                CHECK(grid.get_cell({2, 0}).get_type() == soak_overflow::grid::CellType::high_crate);
                CHECK(grid.get_cell({0, 1}).get_type() == soak_overflow::grid::CellType::low_crate);
                CHECK(grid.get_cell({1, 1}).get_type() == soak_overflow::grid::CellType::empty);
                CHECK(grid.get_cell({2, 1}).get_type() == soak_overflow::grid::CellType::low_crate);
                CHECK(grid.get_cell({0, 2}).get_type() == soak_overflow::grid::CellType::high_crate);
                CHECK(grid.get_cell({1, 2}).get_type() == soak_overflow::grid::CellType::low_crate);
                CHECK(grid.get_cell({2, 2}).get_type() == soak_overflow::grid::CellType::empty);
                CHECK(grid.get_cell({0, 3}).get_type() == soak_overflow::grid::CellType::low_crate);
                CHECK(grid.get_cell({1, 3}).get_type() == soak_overflow::grid::CellType::high_crate);
                CHECK(grid.get_cell({2, 3}).get_type() == soak_overflow::grid::CellType::low_crate);
            }
        }
    }
}
