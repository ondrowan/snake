#include "berry.h"
#include "point.h"

Berry::Berry(int x, int y)
{
    coordinates.push_back(Point{ x, y, Material::BERRY });
}

Berry spawnBerry(Point from, Point to)
{
    srand(time(NULL));
    int x = rand() % (to.x - 1) + (from.x + 1);
    int y = rand() % (to.y - 1) + (from.y + 1);

    return Berry(x, y);
}
