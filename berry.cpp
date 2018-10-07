#include <random>
#include "berry.h"

Berry::Berry(int x, int y)
{
    coordinates.push_back(Point{ x, y, Material::BERRY });
}

Berry spawnBerry(std::vector<Point> emptyCoordinates)
{
    std::mt19937 rng;
    rng.seed(std::random_device()());

    std::uniform_int_distribution<std::mt19937::result_type> dist(0, emptyCoordinates.size() - 1);

    auto point = emptyCoordinates[dist(rng)];

    return Berry(point.x, point.y);
}
