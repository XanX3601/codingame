#ifndef OLYMBITS_BOT_BOT_H
#define OLYMBITS_BOT_BOT_H

#include <olymbits/action/Action.h>
#include <olymbits/state/State.h>

namespace olymbits::bot
{
    class Bot
    {
    // constructor *************************************************************
    public:
        Bot();

        virtual ~Bot() = default;

    // play ********************************************************************
    public:
        virtual const action::Action& play(
            const state::State& cr_state,
            int player_index
        ) = 0;
    };
}

#endif
