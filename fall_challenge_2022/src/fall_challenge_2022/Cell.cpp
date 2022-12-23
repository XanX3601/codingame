#include <fall_challenge_2022/Cell.h>

namespace fall_challenge_2022
{
    // constructor *************************************************************
    Cell::Cell():
        // info
        __is_buildable(false),
        __is_harvested(false),
        __is_spawnable(false),
        // owner
        __owner(Player::null),
        // recycle
        __has_recycler(false),
        // scrap
        __scrap_amount(0),
        // unit
        __unit_amount(0)
    {
    }

    // info ********************************************************************
    bool Cell::is_buildable() const
    {
        return __is_buildable;
    }

    bool Cell::is_harvested() const
    {
        return __is_harvested;
    }

    bool Cell::is_spawnable() const
    {
        return __is_spawnable;
    }

    void Cell::set_buildable(bool is_buildable)
    {
        __is_buildable = is_buildable;
    }

    void Cell::set_harvested(bool is_harvested)
    {
        __is_harvested = is_harvested;
    }

    void Cell::set_spawnable(bool is_spawnable)
    {
        __is_spawnable = is_spawnable;
    }

    // owner *******************************************************************
    Player Cell::get_owner() const
    {
        return __owner;
    }

    void Cell::set_owner(Player owner)
    {
        __owner = owner;
    }

    // recycler ****************************************************************
    bool Cell::has_recycler() const
    {
        return __has_recycler;
    }

    void Cell::set_recycler(bool has_recycler)
    {
        __has_recycler = has_recycler;
    }

    // scrap *******************************************************************
    int Cell::get_scrap_amount() const
    {
        return __scrap_amount;
    }

    void Cell::set_scrap_amount(int scrap_amount)
    {
        __scrap_amount = scrap_amount;
    }

    // unit ********************************************************************
    int Cell::get_unit_amount() const
    {
        return __unit_amount;
    }

    void Cell::set_unit_amount(int unit_amount)
    {
        __unit_amount = unit_amount;
    }
}
