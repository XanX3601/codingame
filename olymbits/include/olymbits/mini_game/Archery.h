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

    // simulation **************************************************************
    public:
        void apply_to(
            const action::Action& cr_action_player_0,
            const action::Action& cr_action_player_1,
            const action::Action& cr_action_player_2,
            Archery& r_archery
        ) const;

    private:
        void apply_to(
            const action::Action& cr_action,
            int player_index,
            int wind_speed,
            Archery& r_archery
        ) const;

    // wind ********************************************************************
    public:
        std::optional<int> get_current_wind_speed() const;
    };
}

#endif
