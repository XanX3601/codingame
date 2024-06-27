#include <olymbits/mini_game/MiniGamePool.h>

namespace olymbits::mini_game
{
    // constructor *************************************************************
    MiniGamePool::MiniGamePool():
        __archery_stack(),
        __diving_stack(),
        __hurdle_race_stack(),
        __roller_speed_skating()
    {
    }

    // instance ****************************************************************
    MiniGamePool& MiniGamePool::get_mini_game_pool()
    {
        static MiniGamePool mini_game_pool;
        return mini_game_pool;
    }

    // mini game ***************************************************************
    std::unique_ptr<Archery> MiniGamePool::get_archery()
    {
        MiniGamePool& r_mini_game_pool(get_mini_game_pool());
        std::stack<std::unique_ptr<Archery>>& r_archery_stack(
            r_mini_game_pool.__archery_stack
        );

        if (!r_archery_stack.empty())
        {
            std::unique_ptr<Archery> up_archery(
                std::move(r_archery_stack.top())
            );
            r_archery_stack.pop();
            return up_archery;
        }

        return std::make_unique<Archery>();
    }

    std::unique_ptr<Diving> MiniGamePool::get_diving()
    {
        MiniGamePool& r_mini_game_pool(get_mini_game_pool());
        std::stack<std::unique_ptr<Diving>>& r_diving_stack(
            r_mini_game_pool.__diving_stack
        );

        if (!r_diving_stack.empty())
        {
            std::unique_ptr<Diving> up_diving(
                std::move(r_diving_stack.top())
            );
            r_diving_stack.pop();
            return up_diving;
        }

        return std::make_unique<Diving>();
    }

    std::unique_ptr<HurdleRace> MiniGamePool::get_hurdle_race()
    {
        MiniGamePool& r_mini_game_pool(get_mini_game_pool());
        std::stack<std::unique_ptr<HurdleRace>>& r_hurdle_race_stack(
            r_mini_game_pool.__hurdle_race_stack
        );

        if (!r_hurdle_race_stack.empty())
        {
            std::unique_ptr<HurdleRace> up_hurdle_race(
                std::move(r_hurdle_race_stack.top())
            );
            r_hurdle_race_stack.pop();
            return up_hurdle_race;
        }

        return std::make_unique<HurdleRace>();
    }

    std::unique_ptr<RollerSpeedSkating> MiniGamePool::get_roller_speed_skating()
    {
        MiniGamePool& r_mini_game_pool(get_mini_game_pool());
        std::stack<std::unique_ptr<RollerSpeedSkating>>& r_roller_speed_skating_stack(
            r_mini_game_pool.__roller_speed_skating
        );

        if (!r_roller_speed_skating_stack.empty())
        {
            std::unique_ptr<RollerSpeedSkating> up_roller_speed_skating(
                std::move(r_roller_speed_skating_stack.top())
            );
            r_roller_speed_skating_stack.pop();
            return up_roller_speed_skating;
        }

        return std::make_unique<RollerSpeedSkating>();
    }

    void MiniGamePool::give_archery(std::unique_ptr<Archery>&& rv_up_archery)
    {
        get_mini_game_pool().__archery_stack.push(std::move(rv_up_archery));
    }

    void MiniGamePool::give_diving(std::unique_ptr<Diving>&& rv_up_diving)
    {
        get_mini_game_pool().__diving_stack.push(std::move(rv_up_diving));
    }

    void MiniGamePool::give_hurdle_race(
        std::unique_ptr<HurdleRace>&& rv_up_hurdle_race
    )
    {
        get_mini_game_pool().__hurdle_race_stack.push(
            std::move(rv_up_hurdle_race)
        );
    }

    void MiniGamePool::give_roller_speed_skating(
        std::unique_ptr<RollerSpeedSkating>&& rv_up_roller_speed_skating
    )
    {
        get_mini_game_pool().__roller_speed_skating.push(
            std::move(rv_up_roller_speed_skating)
        );
    }
}
