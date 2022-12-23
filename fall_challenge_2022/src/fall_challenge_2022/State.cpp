#include <fall_challenge_2022/State.h>

#include <iostream>

namespace fall_challenge_2022
{
    // constructor *************************************************************
    State::State(
        int width,
        int height
    ):
        // map 
        __map(
            width,
            height
        ),
        // matter 
        __player_to_matter()
    {
    }

    State::State(
        int my_matter,
        int opponent_matter,
        const std::string& cr_map_string
    ):
        // map
        __map(cr_map_string),
        // matter
        __player_to_matter{
            {Player::me, my_matter},
            {Player::opponent, opponent_matter}
        }
    {
    }

    // map *********************************************************************
    const Map& State::get_map() const
    {
        return __map;
    }

    // update ******************************************************************
    void State::update_from_std_in()
    {
        int my_matter;
        int opponent_matter;

        std::cin >> my_matter >> opponent_matter;
        std::cin.ignore();

        __player_to_matter[Player::me] = my_matter;
        __player_to_matter[Player::opponent] = opponent_matter;

        int width(__map.get_width());
        int height(__map.get_height());

        for (int y(0); y < height; ++y)
        {
            for (int x(0); x < width; ++x)
            {
                int scrap_amount;
                int owner;
                int unit_amount;
                int recycler;
                int is_buildable;
                int is_spawnable;
                int is_harvested;

                std::cin >> scrap_amount >> owner >> unit_amount >> recycler >> is_buildable >> is_spawnable >> is_harvested;
                std::cin.ignore();

                Cell& r_cell(__map.get_cell(x, y));
                r_cell.set_scrap_amount(scrap_amount);
                r_cell.set_owner(static_cast<Player>(owner));
                r_cell.set_unit_amount(unit_amount);
                r_cell.set_harvested(static_cast<bool>(recycler));
                r_cell.set_buildable(static_cast<bool>(is_buildable));
                r_cell.set_spawnable(static_cast<bool>(is_spawnable));
                r_cell.set_harvested(static_cast<bool>(is_harvested));
            }
        }
    }
}
