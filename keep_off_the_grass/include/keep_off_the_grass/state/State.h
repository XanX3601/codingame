#ifndef KEEP_OFF_THE_GRASS_STATE_STATE_H
#define KEEP_OFF_THE_GRASS_STATE_STATE_H

#include <unordered_map>
#include <vector>

#include <keep_off_the_grass/map/Map.h>
#include <keep_off_the_grass/player/Player.h>

namespace keep_off_the_grass::state
{
    class State
    {
    // cell ********************************************************************
    public:
        const std::vector<
            std::reference_wrapper<const map::Cell>
        >& get_player_cells(
            player::Player player
        ) const;

        const std::vector<std::reference_wrapper<
            const map::Cell>
        >& get_player_units(
            player::Player player
        ) const;

    private:
        std::unordered_map<
            player::Player,
            std::vector<std::reference_wrapper<const map::Cell>>
        > __player_to_cells;

        std::unordered_map<
            player::Player,
            std::vector<std::reference_wrapper<const map::Cell>>
        > __player_to_units;

    // constructor *************************************************************
    public:
        State(
            int width,
            int height
        );

        State(const State&) = delete;

        State(State&&) = default;

    // map *********************************************************************
    public:
        const map::Map& get_map() const;

    private:
        map::Map __map;

    // matter ******************************************************************
    public:
        int get_player_matter_count(player::Player player) const;

    private:
        std::unordered_map<player::Player, int> __player_to_matter_count;

    // update ******************************************************************
    public:
        void update_from_stdin();
    };
}

#endif // KEEP_OFF_THE_GRASS_STATE_STATE_H
