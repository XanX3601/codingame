#ifndef KEEP_OFF_THE_GRASS_PLAYER_PLAYER_H
#define KEEP_OFF_THE_GRASS_PLAYER_PLAYER_H

#include <vector>

namespace keep_off_the_grass::player
{
    enum class Player: int
    {
        no_one = -1,
        opponent = 0,
        me = 1
    };

    extern std::vector<Player> players;
}

#endif // KEEP_OFF_THE_GRASS_PLAYER_PLAYER_H
