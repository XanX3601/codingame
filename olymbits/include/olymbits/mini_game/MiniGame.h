#ifndef OLYMBITS_MINI_GAME_MINI_GAME_H
#define OLYMBITS_MINI_GAME_MINI_GAME_H

#include <array>
#include <cstdlib>
#include <string>

namespace olymbits::mini_game
{
    class MiniGame
    {
    // constructor *********************************************************
    public:
        MiniGame();

    // registers ***********************************************************
    public:
        const std::string& get_gpu() const;

        int get_register(std::size_t register_index) const;

        static const std::size_t register_count;

    private:
        std::string __gpu;

        std::array<int, 7> __registers;
    };
}

#endif
