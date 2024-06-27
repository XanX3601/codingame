#ifndef OLYMBITS_SCORE_BOARD_SCORE_BOARD_H
#define OLYMBITS_SCORE_BOARD_SCORE_BOARD_H

#include <vector>

namespace olymbits::score_board
{
    class ScoreBoard
    {
    // constructor *************************************************************
    public:
        ScoreBoard(int game_count);

    // final score *************************************************************
    private:
        std::vector<int> __final_scores;

    // game count **************************************************************
    private:
        int __game_count;

    // medals ******************************************************************
    public:
        int get_player_score(int player, int mini_game_index) const;

    private:
        std::vector<int> __medals;

    // update ******************************************************************
    public:
        void update();
    };
}

#endif
