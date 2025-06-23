#ifndef OLYMBITS_STATE_STATE_H
#define OLYMBITS_STATE_STATE_H

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
        State();

    // mini games **************************************************************
    public:
        const mini_game::Archery& get_archery() const;

        const mini_game::Diving& get_diving() const;

        const mini_game::HurdleRace& get_hurdle_race() const;

        const mini_game::RollerSpeedSkating& get_roller_speed_skating() const;

    private:
        mini_game::Archery __archery;
        mini_game::Diving __diving;
        mini_game::HurdleRace __hurdle_race;
        mini_game::RollerSpeedSkating __roller_speed_skating;

    // score board *************************************************************
    public:
        const score_board::ScoreBoard& get_score_board() const;

    private:
        score_board::ScoreBoard __score_board;

    // simulation **************************************************************
    public:
        void apply_but_roller_speed_skating_to(
            const action::Action& cr_action_player_0,
            const action::Action& cr_action_player_1,
            const action::Action& cr_action_player_2,
            State& r_state) const;

    // update ******************************************************************
    public:
        void update();
    };
}

#endif
