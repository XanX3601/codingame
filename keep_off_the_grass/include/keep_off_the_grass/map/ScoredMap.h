#ifndef KEEP_OFF_THE_GRASS_MAP_SCORED_MAP_H
#define KEEP_OFF_THE_GRASS_MAP_SCORED_MAP_H

#include <functional>
#include <optional>
#include <vector>

#include <keep_off_the_grass/map/Map.h>

namespace keep_off_the_grass::map
{
    class ScoredMap
    {
    // cell ********************************************************************
    public:
        const Cell& get_max_neighbor(const Cell& cr_cell) const;

        const Cell& get_max_neighbor(int x, int y) const;
        
    // constructor *************************************************************
    public:
        ScoredMap(
            const Map& cr_map
        );

    // map *********************************************************************
    private:
        const Map& __cr_map;

    // score *******************************************************************
    public:
        void add_score(int x, int y, double score);

        void add_score_and_propagate(
            const Cell& cr_cell,
            double score,
            const std::function<double(double)>& cr_propagate_function
        );

        void add_score_and_propagate(
            int x,
            int y,
            double score,
            const std::function<double(double)>& cr_propagate_function
        );

        double get_score(const Cell& cr_cell) const;

        double get_score(int x, int y) const;

    private:
        int cell_to_index(const Cell& cr_cell) const;

        int coords_to_index(int x, int y) const;

        std::vector<double> __scores;

    // scored map **************************************************************
    public:
        ScoredMap add_scored_map(
            const ScoredMap& cr_scored_map,
            double factor=1
        );

        void add_scored_map_in_place(
            const ScoredMap& cr_scored_map,
            double factor=1
        );
    };
}

#endif // KEEP_OFF_THE_GRASS_MAP_SCORED_MAP_H
