#ifndef OLYMBITS_MINI_GAME_ARCHERY_H
#define OLYMBITS_MINI_GAME_ARCHERY_H

#include <optional>

#include <olymbits/mini_game/MiniGame.h>

namespace olymbits::mini_game
{
    class Archery: public MiniGame
    {
    // constructor *************************************************************
    public:
        Archery();

    // player ******************************************************************
    public:
        int get_player_x_position(int player_index) const;

        int get_player_y_position(int player_index) const;

    // wind ********************************************************************
    public:
        std::optional<int> get_current_wind_speed() const;
    };
}

#endif
