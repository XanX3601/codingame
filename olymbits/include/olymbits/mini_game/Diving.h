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
    };
}

#endif
