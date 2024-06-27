#include <olymbits/tools/distance.h>

#include <cmath>

namespace olymbits::tools
{
    int compute_manhattan_distance(int x1, int y1, int x2, int y2)
    {
        return std::abs(x1 - x2) + std::abs(y1 - y2);
    }
}
