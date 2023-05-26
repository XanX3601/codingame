#include <keep_off_the_grass/player/Player.h>

namespace keep_off_the_grass::player
{
    std::vector<Player> players{
        Player::no_one,
        Player::opponent,
        Player::me
    };
}
