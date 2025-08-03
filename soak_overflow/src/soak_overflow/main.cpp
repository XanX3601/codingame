#include <iostream>

#include <soak_overflow/agent/AgentDataCollection.h>
#include <soak_overflow/grid/Grid.h>
#include <soak_overflow/profiler/Profiler.h>
#include <soak_overflow/state/State.h>

int main() {
    int my_id;
    std::cin >> my_id;
    std::cin.ignore();

    soak_overflow::agent::AgentDataCollection agent_data_collection(
        soak_overflow::agent::AgentDataCollection::from_stdin()
    );

    int width;
    int height;
    std::cin >> width >> height;
    std::cin.ignore();
    soak_overflow::grid::Grid grid(width, height);
    grid.update_from_stdin();

    soak_overflow::state::State state(&agent_data_collection);

    while (true)
    {
        state.update_from_stdin();

        int _;
        std::cin >> _;
        std::cin.ignore();

        for (int agent_id: agent_data_collection.get_player_agent_ids(my_id))
        {
             std::cout << "MOVE 0 0" << std::endl;
        }

        soak_overflow::profiler::Profiler::report_and_reset();
    }
}
