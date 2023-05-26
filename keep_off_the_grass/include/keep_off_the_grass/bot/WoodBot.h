#ifndef KEEP_OFF_THE_GRASS_BOT_WOOD_BOT_H
#define KEEP_OFF_THE_GRASS_BOT_WOOD_BOT_H

#include <unordered_map>

#include <keep_off_the_grass/bot/Bot.h>

namespace keep_off_the_grass::bot
{
    class WoodBot: public Bot
    {
    // constructor *************************************************************
    public:
        WoodBot();

    // parameter ***************************************************************
    private:
        std::unordered_map<player::Player, double> __player_to_cell_score;

    // play ********************************************************************
    public:
        std::vector<std::unique_ptr<action::Action>> play(
            const state::State& cr_state
        ) override;
    };
}

#endif // KEEP_OFF_THE_GRASS_BOT_WOOD_BOT_H
