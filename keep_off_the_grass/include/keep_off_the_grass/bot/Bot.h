#ifndef KEEP_OFF_THE_GRASS_BOT_BOT_H
#define KEEP_OFF_THE_GRASS_BOT_BOT_H

#include <memory>
#include <vector>

#include <keep_off_the_grass/action/Action.h>
#include <keep_off_the_grass/state/State.h>

namespace keep_off_the_grass::bot
{
    class Bot
    {
    // constructor *************************************************************
    public:
        Bot(
        );

        virtual ~Bot() = default;

    // play ********************************************************************
    public:
        virtual std::vector<std::unique_ptr<action::Action>> play(
            const state::State& cr_state
        ) = 0;
    };
}

#endif // KEEP_OFF_THE_GRASS_BOT_BOT_H
