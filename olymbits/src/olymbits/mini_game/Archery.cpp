#include <olymbits/mini_game/Archery.h>

namespace olymbits::mini_game
{
    // constructor *************************************************************
    Archery::Archery():
        MiniGame()
    {
    }

    // player ******************************************************************
    int Archery::get_player_x_position(int player_index) const
    {
        return __registers[player_index * 2];
    }

    int Archery::get_player_y_position(int player_index) const
    {
        return __registers[player_index * 2 + 1];
    }

    // simulation **************************************************************
    void Archery::apply_to(
        const action::Action& cr_action_player_0,
        const action::Action& cr_action_player_1,
        const action::Action& cr_action_player_2,
        Archery& r_archery
    ) const
    {
        if (__is_game_over)
        {
            r_archery = *this;
            return;
        }

        int wind_speed(get_current_wind_speed().value());

        apply_to(cr_action_player_0, 0, wind_speed, r_archery);
        apply_to(cr_action_player_1, 1, wind_speed, r_archery);
        apply_to(cr_action_player_2, 2, wind_speed, r_archery);

        r_archery.__gpu = __gpu.substr(1);

        if (r_archery.__gpu.size() == 0)
        {
            r_archery.__is_game_over = true;
        }
    }

    void Archery::apply_to(
        const action::Action& cr_action,
        int player_index,
        int wind_speed,
        Archery& r_archery
    ) const
    {
        int player_x_index(player_index * 2);
        int player_y_index(player_x_index + 1);

        if (
            (cr_action == action::Action::LEFT)
            || cr_action == action::Action::UP
        )
        {
            wind_speed *= -1;
        }

        if (
            (cr_action == action::Action::LEFT)
            || cr_action == action::Action::RIGHT
        )
        {
            r_archery.__registers[player_x_index] = (
                __registers[player_x_index] + wind_speed
            );
            r_archery.__registers[player_y_index] = __registers[player_y_index];
        }
        else
        {
            r_archery.__registers[player_x_index] = __registers[player_x_index];
            r_archery.__registers[player_y_index] = (
                __registers[player_y_index] + wind_speed
            );
        }
    }

    // wind ********************************************************************
    std::optional<int> Archery::get_current_wind_speed() const
    {
        if (__is_game_over)
        {
            return std::nullopt;
        }

        std::optional<int> optional_current_wind_speed(
            __gpu[0] - '0'
        );

        return optional_current_wind_speed;
    }
}
