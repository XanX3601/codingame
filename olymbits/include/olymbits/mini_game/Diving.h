#ifndef OLYMBITS_MINI_GAME_DIVING_H
#define OLYMBITS_MINI_GAME_DIVING_H

#include <olymbits/mini_game/MiniGame.h>

namespace olymbits::mini_game
{
    class Diving: public MiniGame
    {
    // constructor *************************************************************
    public:
        Diving();

    // diving goal *************************************************************
    public:
        char get_next_action_first_letter() const;

    // simulation **************************************************************
    public:
        void apply_to(
            const action::Action& cr_action_player_0,
            const action::Action& cr_action_player_1,
            const action::Action& cr_action_player_2,
            Diving& r_diving
        ) const;

    private:
        void apply_to(
            const action::Action& cr_action,
            int player_index,
            char next_action_first_letter,
            Diving& r_diving
        ) const;
    };
}

#endif
