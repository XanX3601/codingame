#include <doctest/doctest.h>

#include <soak_overflow/grid/Cell.h>

TEST_CASE("soak_overflow::grid::Cell")
{
    SUBCASE("Given a cell with coord (2, 5)")
    {
        soak_overflow::grid::Cell cell({2, 5});

        SUBCASE("Then its default type is empty")
        {
            CHECK(cell.get_type() == soak_overflow::grid::CellType::empty);
        }

        SUBCASE("When setting its type to low_crate")
        {
            cell.set_type(soak_overflow::grid::CellType::low_crate);

            SUBCASE("Then its type is low_crate")
            {
                CHECK(cell.get_type() == soak_overflow::grid::CellType::low_crate);
            }
        }

        SUBCASE("When adding neighbor cells (1, 5) and (3, 6) orthogonal")
        {
            soak_overflow::grid::Cell non_orthogonal_neighbor({1, 5});
            soak_overflow::grid::Cell orthogonal_neighbor({3, 6});

            cell.add_neighbor(&non_orthogonal_neighbor, false);
            cell.add_neighbor(&orthogonal_neighbor, true);

            SUBCASE("Then, both are in its neighbors and one is its orthogonal neighbors")
            {
                CHECK(cell.get_neighbors().size() == 2);

                bool orthogonal_neighbor_found(false);
                bool non_orthogonal_neighbor_found(false);
                for (const soak_overflow::grid::Cell* p_neighbor: cell.get_neighbors())
                {
                    if (p_neighbor == &orthogonal_neighbor)
                    {
                        orthogonal_neighbor_found = true;
                    }
                    else if (p_neighbor == &non_orthogonal_neighbor)
                    {
                        non_orthogonal_neighbor_found = true;
                    }
                }

                CHECK(orthogonal_neighbor_found);
                CHECK(non_orthogonal_neighbor_found);

                orthogonal_neighbor_found = false;
                non_orthogonal_neighbor_found = false;

                for (const soak_overflow::grid::Cell* p_orthogonal_neighbor: cell.get_neighbors(true))
                {
                    if (p_orthogonal_neighbor == &orthogonal_neighbor)
                    {
                        orthogonal_neighbor_found = true;
                    }
                    else if (p_orthogonal_neighbor == &non_orthogonal_neighbor)
                    {
                        non_orthogonal_neighbor_found = true;
                    }
                }

                CHECK(orthogonal_neighbor_found);
                CHECK_FALSE(non_orthogonal_neighbor_found);
            }
        }
    }
}
