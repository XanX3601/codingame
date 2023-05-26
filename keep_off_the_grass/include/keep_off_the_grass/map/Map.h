#ifndef KEEP_OFF_THE_GRASS_MAP_MAP_H
#define KEEP_OFF_THE_GRASS_MAP_MAP_H

#include <vector>

#include <keep_off_the_grass/map/Cell.h>

namespace keep_off_the_grass::map
{
    class Map
    {
    // cell ********************************************************************
    public:
        const Cell& get_cell(int x, int y) const;

        Cell& get_cell(int x, int y);

    private:
        int __coordinates_to_cell_index(
            int x,
            int y
        ) const;

        std::vector<Cell> __cells;

    // constructor *************************************************************
    public:
        Map(
            int width,
            int height
        );

    // size ********************************************************************
    public:
        int get_cell_count() const;

        int get_height() const;

        int get_width() const;

    private:

        int __height;

        int __width;
    };
}

#endif // KEEP_OFF_THE_GRASS_MAP_MAP_H
