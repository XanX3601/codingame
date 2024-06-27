#include <olymbits/mini_game/Archery.h>

#include <olymbits/mini_game/MiniGamePool.h>

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

    // wind ********************************************************************
    std::optional<int> Archery::get_current_wind_speed() const
    {
        if (is_game_over())
        {
            return std::nullopt;
        }

        std::optional<int> optional_current_wind_speed(
            __gpu[0] - '0'
        );

        return optional_current_wind_speed;
    }
}
