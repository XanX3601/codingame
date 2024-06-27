#include <olymbits/bot/HeuristicBot.h>

namespace olymbits::bot
{
    // constructor *************************************************************
    HeuristicBot::HeuristicBot():
        Bot(),
        // bots
        __archery_bot(),
        __diving_bot(),
        __hurdle_race_bot(),
        __roller_speed_skating_bot()
    {
    }

    // play ********************************************************************
    const action::Action& HeuristicBot::play(
        const state::State& cr_state,
        int player_index
    )
    {
        const mini_game::Diving& cr_diving(cr_state.get_diving());

        if (!cr_diving.is_game_over())
        {
            return __diving_bot.play(cr_state, player_index);
        }

        const score_board::ScoreBoard& cr_score_board(
            cr_state.get_score_board()
        );

        std::vector<int> other_mini_game_indexes{0, 1, 2};
        std::sort(
            other_mini_game_indexes.begin(),
            other_mini_game_indexes.end(),
            [&](int mini_game_index_a, int mini_game_index_b)
            {
                return (
                    cr_score_board.get_player_score(player_index, mini_game_index_a)
                    <= cr_score_board.get_player_score(player_index, mini_game_index_b)
                );
            }
        );

        int mini_game_index(other_mini_game_indexes[0]);

        if (mini_game_index == 0)
        {
            return __hurdle_race_bot.play(cr_state, player_index);
        }
        else if (mini_game_index == 1)
        {
            return __archery_bot.play(cr_state, player_index);
        }
        
        return __roller_speed_skating_bot.play(cr_state, player_index);
    }
}
