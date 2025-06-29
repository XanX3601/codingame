#ifndef OLYMBITS_MINI_GAME_HURDLE_RACE_H
#define OLYMBITS_MINI_GAME_HURDLE_RACE_H

#include <optional>

#include <olymbits/mini_game/MiniGame.h>

namespace olymbits::mini_game
{
    class HurdleRace: public MiniGame
    {
    // constructor *************************************************************
    public:
        HurdleRace();

    // hurdle ******************************************************************
    public:
        std::optional<int> get_distance_to_next_hurdle(int position_index) const;

    // simulation **************************************************************
    public:
        void apply_to(
            const action::Action& cr_action_player_0,
            const action::Action& cr_action_player_1,
            const action::Action& cr_action_player_2,
            HurdleRace& r_hurdle_race
        ) const;

    private:
        void apply_to(
            const action::Action& cr_action,
            int player_index,
            HurdleRace& r_hurdle_race
        ) const;

    // player ******************************************************************
    public:
        int get_player_position_index(int player_index) const;
    };
}

#endif
