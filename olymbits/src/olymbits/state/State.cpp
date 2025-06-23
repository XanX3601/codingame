#include <olymbits/state/State.h>

namespace olymbits::state
{
    // constructor *************************************************************
    State::State():
        // mini games
        __archery(),
        __diving(),
        __hurdle_race(),
        __roller_speed_skating(),
        // score board
        __score_board()
    {
    }

    // mini games **************************************************************
    const mini_game::Archery& State::get_archery() const
    {
        return __archery;
    }

    const mini_game::Diving& State::get_diving() const
    {
        return __diving;
    }

    const mini_game::HurdleRace& State::get_hurdle_race() const
    {
        return __hurdle_race;
    }

    const mini_game::RollerSpeedSkating& State::get_roller_speed_skating() const
    {
        return __roller_speed_skating;
    }

    // score board *************************************************************
    const score_board::ScoreBoard& State::get_score_board() const
    {
        return __score_board;
    }

    // simulation **************************************************************
    void State::apply_but_roller_speed_skating_to(
        const action::Action& cr_action_player_0,
        const action::Action& cr_action_player_1,
        const action::Action& cr_action_player_2,
        State& r_state
    ) const
    {
        
    }

    // update ******************************************************************
    void State::update()
    {
        __score_board.update();
        __hurdle_race.update();
        __archery.update();
        __roller_speed_skating.update();
        __diving.update();
    }
}
