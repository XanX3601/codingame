#include <fall_challenge_2022/Map.h>

#include <sstream>

namespace fall_challenge_2022
{
    // cell ********************************************************************
    const Cell& Map::get_cell(int x, int y) const
    {
        return __cells[coords_to_index(x, y)];
    }

    Cell& Map::get_cell(int x, int y)
    {
        return __cells[coords_to_index(x, y)];
    }

    int Map::coords_to_index(int x, int y) const
    {
        return y * __width + x;
    }

    // constructor *************************************************************
    Map::Map(
        int width,
        int height
    ):
        // cell
        __cells(width * height, Cell()),
        // dimension 
        __height(height),
        __width(width)
    {
    }

    Map::Map(
        const std::string& cr_map_string
    ):
        // cell
        __cells(),
        // dimension
        __height(0),
        __width(0)
    {
        std::string dimension_string(
            cr_map_string.substr(0, cr_map_string.find("-"))
        );

        std::string width_string(
            dimension_string.substr(0, dimension_string.find("x"))
        );
        std::string height_string(
            dimension_string.substr(width_string.size() + 1)
        );

        int width(std::stoi(width_string));
        int height(std::stoi(height_string));

        __width = width;
        __height = height;

        __cells.resize(width * height, Cell());

        std::string cells_string(
            cr_map_string.substr(dimension_string.size() + 1)
        );
        for (int cell_index(0); cell_index < width * height; ++cell_index)
        {
            auto separator_position(cells_string.find(";"));
            std::string cell_string(cells_string.substr(0, separator_position));
            cells_string.erase(0, separator_position + 1);

            int x;
            int y;
            int scrap_amount;
            Player owner;
            int unit_amount;
            bool has_recycler;
            bool is_buildable;
            bool is_spawnable;
            bool is_harvested;

            for (int field_index(0); field_index < 9; ++field_index)
            {
                auto separator_position(cell_string.find(","));
                std::string field_string(
                    cell_string.substr(0, separator_position)
                );
                cell_string.erase(0, separator_position + 1);

                if (field_index == 0)
                {
                    x = std::stoi(field_string);
                }
                else if (field_index == 1)
                {
                    y = std::stoi(field_string);
                }
                else if (field_index == 2)
                {
                    scrap_amount = std::stoi(field_string);
                }
                else if (field_index == 3)
                {
                    owner = static_cast<Player>(std::stoi(field_string));
                }
                else if (field_index == 4)
                {
                    unit_amount = std::stoi(field_string);
                }
                else if (field_index == 5)
                {
                    has_recycler = std::stoi(field_string) == 1;
                }
                else if (field_index == 6)
                {
                    is_buildable = std::stoi(field_string) == 1;
                }
                else if (field_index == 7)
                {
                    is_spawnable = std::stoi(field_string) == 1;
                }
                else
                {
                    is_harvested = std::stoi(field_string) == 1;
                }
            }

            Cell& r_cell(get_cell(x, y));
            r_cell.set_scrap_amount(scrap_amount);
            r_cell.set_owner(owner);
            r_cell.set_unit_amount(unit_amount);
            r_cell.set_recycler(has_recycler);
            r_cell.set_buildable(is_buildable);
            r_cell.set_spawnable(is_spawnable);
            r_cell.set_harvested(is_harvested);
        }
    }

    // dimension ***************************************************************
    int Map::get_height() const
    {
        return __height;
    }

    int Map::get_width() const
    {
        return __width;
    }

    // string ******************************************************************
    std::string Map::to_string() const
    {
        std::stringstream map_stringstream;

        map_stringstream
            << __width
            << "x"
            << __height
            << "-";

        for (int x(0); x < __width; ++x)
        {
            for (int y(0); y < __height; ++y)
            {
                const Cell& cr_cell(get_cell(x, y));

                map_stringstream
                    << x << ","
                    << y << ","
                    << cr_cell.get_scrap_amount() << ","
                    << static_cast<int>(cr_cell.get_owner()) << ","
                    << cr_cell.get_unit_amount() << ","
                    << (cr_cell.has_recycler() ? 1 : 0) << ","
                    << (cr_cell.is_buildable() ? 1 : 0) << ","
                    << (cr_cell.is_spawnable() ? 1 : 0) << ","
                    << (cr_cell.is_harvested() ? 1 : 0) << ";";
            }
        }

        std::string map_string(map_stringstream.str());
        map_string.pop_back();

        return map_string;
    }
}
