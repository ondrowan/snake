#include "berry.h"
#include "point.h"

Berry::Berry(int x, int y)
{
    coordinates.push_back(Point{ x, y, Material::BERRY });
}

Berry spawnBerry(Point from, Point to)
{
    int x = rand() % to.x + from.x - 1;
    int y = rand() % to.y + from.y - 1;

    return Berry(x, y);
}