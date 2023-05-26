#include <keep_off_the_grass/state/State.h>

#include <iostream>

namespace keep_off_the_grass::state
{
    // cell ********************************************************************
    const std::vector<
        std::reference_wrapper<const map::Cell>
    >& State::get_player_cells(
        player::Player player
    ) const
    {
        return __player_to_cells.at(player);
    }

    const std::vector<
        std::reference_wrapper<const map::Cell>
    >& State::get_player_units(
        player::Player player
    ) const
    {
        return __player_to_units.at(player);
    }

    // constructor *************************************************************
    State::State(
        int width,
        int height
    ):
        // cell
        __player_to_cells{
            {player::Player::no_one,   {}},
            {player::Player::opponent, {}},
            {player::Player::me,       {}}
        },
        __player_to_units{
            {player::Player::no_one,   {}},
            {player::Player::opponent, {}},
            {player::Player::me,       {}}
        },
        // map
        __map(width, height),
        // matter
        __player_to_matter_count{
            {player::Player::opponent, 0},
            {player::Player::me,       0}
        }
    {
        for (player::Player player: player::players)
        {
            __player_to_cells[player].reserve(__map.get_cell_count());
            __player_to_units[player].reserve(__map.get_cell_count());
        }

        for (int x(0); x < __map.get_width(); ++x)
        {
            for (int y(0); y < __map.get_height(); ++y)
            {
                const map::Cell& cr_cell(__map.get_cell(x, y));

                __player_to_cells[player::Player::no_one].emplace_back(cr_cell);
            }
        }
    }

    // map *********************************************************************
    const map::Map& State::get_map() const
    {
        return __map;
    }

    // matter ******************************************************************
    int State::get_player_matter_count(player::Player player) const
    {
        return __player_to_matter_count.at(player);
    }

    // update ******************************************************************
    void State::update_from_stdin()
    {
        int my_matter;
        int opponent_matter;

        std::cin >> my_matter >> opponent_matter;
        std::cin.ignore();

        __player_to_matter_count[player::Player::me] = my_matter;
        __player_to_matter_count[player::Player::opponent] = opponent_matter;

        for (player::Player player: player::players)
        {
            __player_to_cells[player].clear();
            __player_to_units[player].clear();
        }

        for (int y(0); y < __map.get_height(); ++y)
        {
            for (int x(0); x < __map.get_width(); ++x)
            {
                int scrap_amount;
                int owner;
                int units;
                int recycler;
                int can_build;
                int can_spawn;
                int in_range_of_recycler;

                std::cin
                    >> scrap_amount
                    >> owner
                    >> units
                    >> recycler
                    >> can_build
                    >> can_spawn
                    >> in_range_of_recycler;
                std::cin.ignore();

                map::Cell& r_cell(__map.get_cell(x, y));

                r_cell.set_scrap_count(scrap_amount);
                r_cell.set_owner(static_cast<player::Player>(owner));
                r_cell.set_unit_count(units);
                r_cell.set_has_recycler(recycler > 0);
                r_cell.set_is_buildable(can_build > 0);
                r_cell.set_is_spawnable(can_spawn > 0);
                r_cell.set_is_recycled(in_range_of_recycler > 0);

                // buffer cells
                player::Player cell_owner(r_cell.get_owner());
                __player_to_cells[cell_owner].emplace_back(r_cell);
                
                if (r_cell.get_unit_count() > 0)
                {
                    __player_to_units[cell_owner].emplace_back(r_cell);
                }
            }
        }
    }
}
