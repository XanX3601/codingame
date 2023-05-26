#include <keep_off_the_grass/bot/WaitBot.h>
#include <memory>

#include <keep_off_the_grass/action/Wait.h>

namespace keep_off_the_grass::bot
{
    // constructor *************************************************************
    WaitBot::WaitBot(
    ):
        Bot()
    {
    }

    // play ********************************************************************
    std::vector<std::unique_ptr<action::Action>> WaitBot::play(
        const state::State& cr_state
    )
    {
        std::vector<std::unique_ptr<action::Action>> actions;

        actions.emplace_back(std::make_unique<action::Wait>());

        return actions;
    }
}
