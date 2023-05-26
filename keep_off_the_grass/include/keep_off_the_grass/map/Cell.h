#ifndef KEEP_OFF_THE_GRASS_MAP_CELL_H
#define KEEP_OFF_THE_GRASS_MAP_CELL_H

#include <functional>
#include <unordered_map>
#include <vector>

#include <keep_off_the_grass/map/Direction.h>
#include <keep_off_the_grass/player/Player.h>

namespace keep_off_the_grass::map
{
    class Cell
    {
    // constructor *************************************************************
    public:
        Cell(
            int x,
            int y
        );

        Cell(const Cell& cr_cell) = delete;

        Cell(Cell&& rv_cell) = default;

    // coordinate **************************************************************
    public:
        int get_x() const;

        int get_y() const;

    private:
        int __x;
        
        int __y;

    // neighbor  ****************************************************************
    public:
        void add_neighbor(Cell& r_cell, Direction direction);

        const Cell& get_neighbor(Direction direction) const;

        int get_neighbor_count() const;

        const std::vector<std::reference_wrapper<const Cell>>& get_neighbors() const;

    private:
        std::unordered_map<Direction, std::reference_wrapper<Cell>> __direction_to_neighbor;

        std::vector<std::reference_wrapper<const Cell>> __neighbors;

    // owner *******************************************************************
    public:
        player::Player get_owner() const;

        void set_owner(player::Player owner);

    private:
        player::Player __owner;

    // recycler ****************************************************************
    public:
        bool has_recycler() const;

        bool is_buildable() const;

        bool is_recycled() const;

        void set_has_recycler(bool has_recycler);

        void set_is_buildable(bool is_buildable);

        void set_is_recycled(bool is_recycled);

    private:
        bool __has_recycler;

        bool __is_buildable;

        bool __is_recycled;

    // scrap *******************************************************************
    public:
        int get_scrap_count() const;

        void set_scrap_count(int scrap_count);

    private:
        int __scrap_count;
    
    // unit ********************************************************************
    public:
        int get_unit_count() const;
        
        bool is_spawnable() const;

        void set_is_spawnable(bool is_spawnable);

        void set_unit_count(int unit_count);

    private:
        bool __is_spawnable;

        int __unit_count;
    };
}

#endif // KEEP_OFF_THE_GRASS_MAP_CELL_H
