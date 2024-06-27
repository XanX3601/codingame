#include <olymbits/bot/RandomBot.h>

namespace olymbits::bot
{
    // constructor *************************************************************
    RandomBot::RandomBot():
        Bot(),
        // random 
        __distribution(0.0, 1.0),
        __generator(),
        __random_device()
    {
        __generator.seed(__random_device());
    }

    // play ********************************************************************
    const action::Action& RandomBot::play(
        const state::State& cr_state,
        int play_index
    )
    {
        double random(__distribution(__generator));

        if (random < 0.25)
        {
            return action::Action::UP;
        }
        if (random < .50)
        {
            return action::Action::DOWN;
        }
        if (random < .75)
        {
            return action::Action::LEFT;
        }

        return action::Action::RIGHT;
    }
}
