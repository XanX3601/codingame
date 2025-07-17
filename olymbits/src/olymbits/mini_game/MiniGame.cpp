#include <olymbits/mini_game/MiniGame.h>

namespace olymbits::mini_game 
{
    // constructor *************************************************************
    MiniGame::MiniGame():
        // registers
        __gpu("GAME OVER"),
        __registers{0, 0, 0, 0, 0, 0, 0}
    {
    }

    // registers ***************************************************************
    const std::string& MiniGame::get_gpu() const
    {
        return __gpu;
    }

    int MiniGame::get_register(std::size_t register_index) const
    {
        return __registers.at(register_index);
    }

    const std::size_t MiniGame::register_count(7);
}
