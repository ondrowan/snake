#include <cassert>
#include "world.h"

World::World(const Point& from, const Point& to)
{
    assert(to.x > from.x);
    assert(to.y > from.y);

    for (int x = from.x; x < to.x + 1; x++)
    {
        coordinates.push_back(Point{ x, from.y, Material::WALL });
        coordinates.push_back(Point{ x, to.y, Material::WALL });
    }

    for (int y = from.y; y < to.y + 1; y++)
    {
        coordinates.push_back(Point{ from.x, y, Material::WALL });
        coordinates.push_back(Point{ to.x, y, Material::WALL });
    }
}
