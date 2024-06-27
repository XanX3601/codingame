#include <olymbits/score_board/ScoreBoard.h>

#include <iostream>

namespace olymbits::score_board
{
    // constructor *************************************************************
    ScoreBoard::ScoreBoard(int game_count):
        // final score
        __final_scores(3, 0),
        // game count
        __game_count(game_count),
        // medals
        __medals(game_count * 3, 0)
    {
    }

    // medals ******************************************************************
    int ScoreBoard::get_player_score(
        int player_index,
        int mini_game_index
    ) const
    {
        int player_gold_medal_index(
            player_index * __game_count * 3 + mini_game_index * 3
        );
        
        return (
            __medals[player_gold_medal_index] * 3
            + __medals[player_gold_medal_index + 1]
        );
    }

    // update ******************************************************************
    void ScoreBoard::update()
    {
        for (int player_index(0); player_index < 3; ++player_index)
        {
            std::cin >> __final_scores[player_index];

            for (int game_index(0); game_index < __game_count; ++game_index)
            {
                int player_gold_medal_index = player_index * __game_count * 3 + game_index * 3;
                std::cin
                    >> __medals[player_gold_medal_index]
                    >> __medals[player_gold_medal_index + 1]
                    >> __medals[player_gold_medal_index + 2];
            }

            std::cin.ignore();
        }
    }

}
