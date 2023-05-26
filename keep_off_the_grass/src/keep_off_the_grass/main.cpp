#include <iostream>
#include <memory>
#include <sstream>

#include <keep_off_the_grass/action/Action.h>
#include <keep_off_the_grass/bot/WaitBot.h>
#include <keep_off_the_grass/bot/WoodBot.h>
#include <keep_off_the_grass/state/State.h>

int main()
{
    std::unique_ptr<keep_off_the_grass::bot::Bot> up_bot(
        std::make_unique<keep_off_the_grass::bot::WoodBot>()
    );

    int width;
    int height;
    std::cin >> width >> height;
    std::cin.ignore();

    keep_off_the_grass::state::State state(width, height);

    while (true)
    {
        state.update_from_stdin();

        std::vector<std::unique_ptr<keep_off_the_grass::action::Action>> __actions(
            up_bot->play(state)
        );

        std::stringstream action_stream;

        for (const auto& cr_up_action: __actions)
        {
            cr_up_action->add_to_action_stream(action_stream);
        }

        std::cerr << __actions.size() << std::endl;

        std::cout
            << action_stream.str()
            << "\n";
    }
}
