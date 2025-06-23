#include <olymbits/mini_game/MiniGame.h>

#include <iostream>

namespace olymbits::mini_game
{
    // constructor *************************************************************
    MiniGame::MiniGame():
        // game over
        __is_game_over(false),
        // gpu
        __gpu(),
        // registers
        __registers(7, 0)
    {
    }

    // game over ***************************************************************
    bool MiniGame::is_game_over() const
    {
        return __is_game_over;
    }

    // update ******************************************************************
    void MiniGame::update()
    {
        std::cin
            >> this->__gpu
            >> this->__registers[0]
            >> this->__registers[1]
            >> this->__registers[2]
            >> this->__registers[3]
            >> this->__registers[4]
            >> this->__registers[5]
            >> this->__registers[6];
        std::cin.ignore();

        __is_game_over = __gpu == "GAME_OVER";
    }

    void MiniGame::update(
        const std::vector<int>& cr_registers,
        const std::string& cr_gpu
    )
    {
        __registers = cr_registers;
        __gpu = cr_gpu;
    }
}

