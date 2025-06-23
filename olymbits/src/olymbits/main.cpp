#include <iostream>
#include <memory>

#include <olymbits/action/Action.h>
#include <olymbits/bot/RandomBot.h>
#include <olymbits/state/State.h>

int main()
{
    int my_player_index;
    int game_count;

    std::cin >> my_player_index;
    std::cin.ignore();
    std::cin >> game_count;
    std::cin.ignore();

    olymbits::state::State state;
    std::unique_ptr<olymbits::bot::Bot> bot(
        std::make_unique<olymbits::bot::RandomBot>()
    );

    while (true)
    {
        state.update();

        olymbits::action::Action action(bot->play(state, my_player_index));

        std::cout << action << std::endl;
    }
}
