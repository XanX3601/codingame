#include <keep_off_the_grass/action/Move.h>
#include <keep_off_the_grass/bot/WoodBot.h>
#include <keep_off_the_grass/map/ScoredMap.h>

namespace keep_off_the_grass::bot
{
    // constructor *************************************************************
    WoodBot::WoodBot(
    ):
        Bot(),
        // parameter
        __player_to_cell_score{
            {player::Player::no_one,   10},
            {player::Player::me,       25},
            {player::Player::opponent, 25},
        }
    {
    }

    // play ********************************************************************
    std::vector<std::unique_ptr<action::Action>> WoodBot::play(
        const state::State& cr_state
    )
    {
        // get the map ***********************************************
        const map::Map& cr_map(cr_state.get_map());

        // create scored map *****************************************
        for (player::Player player: player::players)
        {
            double cell_score(__player_to_cell_score[player]);

            for (
                const std::reference_wrapper<const map::Cell> cr_rw_cell:
                cr_state.get_player_cells(player)
            )
            {
                const map::Cell& cr_cell(cr_rw_cell.get());

            }
        }

        // create actions list ***************************************
        std::vector<std::unique_ptr<action::Action>> actions;

        // move actions **********************************************
        map::ScoredMap move_scored_map(cr_map);

        for (const auto& cr_rw_cell: cr_state.get_player_units(player::Player::me))
        {
            const map::Cell& cr_cell(cr_rw_cell.get());

            if (cr_cell.get_neighbor_count() == 0)
            {
                continue;
            }

            const map::Cell& cr_neighbor = move_scored_map.get_max_neighbor(cr_cell);

            actions.emplace_back(
                std::make_unique<action::Move>(
                    cr_cell.get_unit_count(),
                    cr_cell.get_x(),
                    cr_cell.get_y(),
                    cr_neighbor.get_x(),
                    cr_neighbor.get_y()
                )
            );
        }

        // return ****************************************************
        return actions;
    }
}
