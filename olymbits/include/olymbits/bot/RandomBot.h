#ifndef OLYMBITS_BOT_RANDOM_BOT_H
#define OLYMBITS_BOT_RANDOM_BOT_H

#include <random>

#include <olymbits/bot/Bot.h>

namespace olymbits::bot
{
    class RandomBot: public Bot
    {
    // constructor *************************************************************
    public:
        RandomBot();

    // play ********************************************************************
    public:
        const action::Action& play(
            const state::State& cr_state,
            int player_index
        );

    // random ******************************************************************
    private:
        std::uniform_real_distribution<double> __distribution;

        std::mt19937 __generator;

        std::random_device __random_device;
    };
}

#endif
