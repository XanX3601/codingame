#include <olymbits/mini_game/Diving.h>

#include <iostream>

namespace olymbits::mini_game
{
    // constructor *************************************************************
    Diving::Diving():
        MiniGame()
    {
    }

    // diving goal *************************************************************
    char Diving::get_next_action_first_letter() const
    {
        return __gpu[0];
    }
}
