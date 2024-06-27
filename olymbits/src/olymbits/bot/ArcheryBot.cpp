#include <olymbits/bot/ArcheryBot.h>

#include <olymbits/tools/distance.h>

namespace olymbits::bot
{
    // constructor *************************************************************
    ArcheryBot::ArcheryBot():
        Bot()
    {
    }

    // play ********************************************************************
    const action::Action& ArcheryBot::play(
        const state::State& cr_state,
        int player_index
    )
    {
        const mini_game::Archery& cr_archery(cr_state.get_archery());

        std::optional<int> optional_current_wind_speed(
            cr_archery.get_current_wind_speed()
        );

        if (!optional_current_wind_speed.has_value())
        {
            return action::Action::LEFT;
        }

        const int& cr_current_wind_speed(optional_current_wind_speed.value());

        int player_x_position(cr_archery.get_player_x_position(player_index));
        int player_y_position(cr_archery.get_player_y_position(player_index));

        int up_distance (
            tools::compute_manhattan_distance(
                0, 0,
                player_x_position, player_y_position - cr_current_wind_speed
            )
        );
        int down_distance (
            tools::compute_manhattan_distance(
                0, 0,
                player_x_position, player_y_position + cr_current_wind_speed
            )
        );
        int left_distance (
            tools::compute_manhattan_distance(
                0, 0,
                player_x_position - cr_current_wind_speed, player_y_position
            )
        );
        int right_distance (
            tools::compute_manhattan_distance(
                0, 0,
                player_x_position + cr_current_wind_speed, player_y_position
            )
        );

        if (
            up_distance <= down_distance
            && up_distance <= left_distance
            && up_distance <= right_distance
        )
        {
            return action::Action::UP;
        }
        else if (
            down_distance <= up_distance
            && down_distance <= left_distance
            && down_distance <= right_distance
        )
        {
            return action::Action::DOWN;
        }
        else if (
            left_distance <= up_distance
            && left_distance <= down_distance
            && left_distance <= right_distance
        )
        {
            return action::Action::LEFT;
        }

        return action::Action::RIGHT;
    }
}
