#ifndef OLYMBITS_MINI_GAME_MINI_GAME_H
#define OLYMBITS_MINI_GAME_MINI_GAME_H

#include <string>
#include <vector>

#include <olymbits/action/Action.h>

namespace olymbits::mini_game
{
    class MiniGame
    {
    // constructor *************************************************************
    public:
        MiniGame();

        MiniGame(const MiniGame& cr_mini_game) = default;

        MiniGame(MiniGame&& rv_mini_game) = default;

        virtual ~MiniGame() = default;

    // game over ***************************************************************
    public:
        bool is_game_over() const;

    protected:
        bool __is_game_over;

    // gpu *********************************************************************
    protected:
        std::string __gpu;

    // operator ****************************************************************
    public:
        MiniGame& operator=(const MiniGame& cr_mini_game) = default;

    // registers ***************************************************************
    protected:
        std::vector<int> __registers;

    // update ******************************************************************
    public:
        virtual void update();

        virtual void update(
            const std::vector<int>& cr_registers,
            const std::string& cr_gpu
        );
    };
}

#endif
