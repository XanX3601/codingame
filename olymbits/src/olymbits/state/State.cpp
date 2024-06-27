#include <olymbits/state/State.h>

#include <olymbits/mini_game/MiniGamePool.h>

namespace olymbits::state
{
    // constructor *************************************************************
    State::State(int game_count):
        // mini games
        __mini_games(),
        // score board
        __score_board(game_count)
    {
        __mini_games.emplace_back(std::move(mini_game::MiniGamePool::get_hurdle_race()));
        __mini_games.emplace_back(std::move(mini_game::MiniGamePool::get_archery()));
        __mini_games.emplace_back(std::move(mini_game::MiniGamePool::get_roller_speed_skating()));
        __mini_games.emplace_back(std::move(mini_game::MiniGamePool::get_diving()));
    }

    // mini games **************************************************************
    const mini_game::Archery& State::get_archery() const
    {
        const mini_game::Archery* cp_archery(
            dynamic_cast<const mini_game::Archery*>(__mini_games[1].get())
        );
        
        return *cp_archery;
    }

    const mini_game::Diving& State::get_diving() const
    {
        const mini_game::Diving* cp_diving(
            dynamic_cast<const mini_game::Diving*>(__mini_games[3].get())
        );
        
        return *cp_diving;
    }

    const mini_game::HurdleRace& State::get_hurdle_race() const
    {
        const mini_game::HurdleRace* cp_hurdle_race(
            dynamic_cast<const mini_game::HurdleRace*>(__mini_games[0].get())
        );
        
        return *cp_hurdle_race;
    }

    const mini_game::RollerSpeedSkating& State::get_roller_speed_skating() const
    {
        const mini_game::RollerSpeedSkating* cp_roller_speed_skating(
            dynamic_cast<const mini_game::RollerSpeedSkating*>(__mini_games[2].get())
        );
        
        return *cp_roller_speed_skating;
    }

    // score board *************************************************************
    const score_board::ScoreBoard& State::get_score_board() const
    {
        return __score_board;
    }

    // update ******************************************************************
    void State::update()
    {
        __score_board.update();

        for (std::unique_ptr<mini_game::MiniGame>& r_up_mini_game: __mini_games)
        {
            r_up_mini_game->update();
        }
    }
}
