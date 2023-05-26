#include <keep_off_the_grass/map/Cell.h>

namespace keep_off_the_grass::map
{
    // constructor *************************************************************
    Cell::Cell(
        int x,
        int y
    ):
        // coordinate
        __x(x),
        __y(y),
        // neighbor
        __direction_to_neighbor(),
        __neighbors(),
        // owner
        __owner(player::Player::no_one),
        // recycler
        __has_recycler(false),
        __is_buildable(false),
        __is_recycled(false),
        // scrap
        __scrap_count(0),
        // unit
        __is_spawnable(false),
        __unit_count(0)
    {
    }

    // coordinate **************************************************************
    int Cell::get_x() const
    {
        return __x;
    }

    int Cell::get_y() const
    {
        return __y;
    }

    // neighbor ****************************************************************
    void Cell::add_neighbor(Cell& r_cell, Direction direction)
    {
        __direction_to_neighbor.emplace(direction, r_cell);
        __neighbors.emplace_back(r_cell);
    }

    const Cell& Cell::get_neighbor(Direction direction) const
    {
        return __direction_to_neighbor.at(direction);
    }

    int Cell::get_neighbor_count() const
    {
        return __neighbors.size();
    }

    const std::vector<std::reference_wrapper<const Cell>>& Cell::get_neighbors() const
    {
        return __neighbors;
    }

    // owner *******************************************************************
    player::Player Cell::get_owner() const
    {
        return __owner;
    }

    void Cell::set_owner(player::Player owner)
    {
        __owner = owner;
    }

    // recycler ****************************************************************
    bool Cell::has_recycler() const
    {
        return __has_recycler;
    }

    bool Cell::is_buildable() const
    {
        return __is_buildable;
    }

    bool Cell::is_recycled() const
    {
        return __is_recycled;
    }

    void Cell::set_has_recycler(bool has_recycler)
    {
        __has_recycler = has_recycler;
    }

    void Cell::set_is_buildable(bool is_buildable)
    {
        __is_buildable = is_buildable;
    }

    void Cell::set_is_recycled(bool is_recycled)
    {
        __is_recycled = is_recycled;
    }

    // scrap *******************************************************************
    int Cell::get_scrap_count() const
    {
        return __scrap_count;
    }

    void Cell::set_scrap_count(int scrap_count) 
    {
        __scrap_count = scrap_count;
    }

    // unit ********************************************************************
    int Cell::get_unit_count() const
    {
        return __unit_count;
    }

    bool Cell::is_spawnable() const
    {
        return __is_spawnable;
    }

    void Cell::set_is_spawnable(bool is_spawnable)
    {
        __is_spawnable = is_spawnable;
    }

    void Cell::set_unit_count(int unit_count)
    {
        __unit_count = unit_count;
    }
}
