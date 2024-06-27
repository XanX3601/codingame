#ifndef OLYMBITS_STATE_STATE_H
#define OLYMBITS_STATE_STATE_H

#include <memory>

#include <olymbits/mini_game/Archery.h>
#include <olymbits/mini_game/Diving.h>
#include <olymbits/mini_game/MiniGame.h>
#include <olymbits/mini_game/HurdleRace.h>
#include <olymbits/mini_game/RollerSpeedSkating.h>
#include <olymbits/score_board/ScoreBoard.h>

namespace olymbits::state
{
    class State
    {
    // constructor *************************************************************
    public:
        State(int game_count);

    // mini games **************************************************************
    public:
        const mini_game::Archery& get_archery() const;

        const mini_game::Diving& get_diving() const;

        const mini_game::HurdleRace& get_hurdle_race() const;

        const mini_game::RollerSpeedSkating& get_roller_speed_skating() const;

    private:
        std::vector<std::unique_ptr<mini_game::MiniGame>> __mini_games;

    // score board *************************************************************
    public:
        const score_board::ScoreBoard& get_score_board() const;

    private:
        score_board::ScoreBoard __score_board;

    // update ******************************************************************
    public:
        void update();
    };
}

#endif
