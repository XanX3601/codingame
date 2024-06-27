#ifndef OLYMBITS_MINI_GAME_MINI_GAME_POOL_H
#define OLYMBITS_MINI_GAME_MINI_GAME_POOL_H

#include <memory>
#include <stack>

#include <olymbits/mini_game/Archery.h>
#include <olymbits/mini_game/Diving.h>
#include <olymbits/mini_game/HurdleRace.h>
#include <olymbits/mini_game/RollerSpeedSkating.h>

namespace olymbits::mini_game
{
    class MiniGamePool
    {
    // constructor *************************************************************
    public:
        MiniGamePool(const MiniGamePool&) = delete;
        
        MiniGamePool& operator=(const MiniGamePool&) = delete;

    private:
        MiniGamePool();
        
        ~MiniGamePool() = default;

    // instance ****************************************************************
    private:
        static MiniGamePool& get_mini_game_pool();

    // mini game ***************************************************************
    public:
        static std::unique_ptr<Archery> get_archery();

        static std::unique_ptr<Diving> get_diving();

        static std::unique_ptr<HurdleRace> get_hurdle_race();

        static std::unique_ptr<RollerSpeedSkating> get_roller_speed_skating();

        static void give_archery(std::unique_ptr<Archery>&& rv_up_archery);

        static void give_diving(std::unique_ptr<Diving>&& rv_up_diving);

        static void give_hurdle_race(
            std::unique_ptr<HurdleRace>&& rv_up_diving
        );

        static void give_roller_speed_skating(
            std::unique_ptr<RollerSpeedSkating>&& rv_up_roller_speed_skating
        );

    private:
        std::stack<std::unique_ptr<Archery>> __archery_stack;

        std::stack<std::unique_ptr<Diving>> __diving_stack;

        std::stack<std::unique_ptr<HurdleRace>> __hurdle_race_stack;

        std::stack<std::unique_ptr<RollerSpeedSkating>> __roller_speed_skating;
    };
}

#endif
