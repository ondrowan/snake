#include <random>
#include "berry.h"

Berry::Berry(int x, int y)
{
    coordinates.push_back(Point{ x, y, Material::BERRY });
}

/**
 * Spawns berry on coordinates between "from" and "to", omitting any part of
 * snake. In case berry is spawned inside of a snake, it's recursively
 * respawned until valid position is found.
 *
 * TODO: I think this might overflow once snake is long enough and there are
 * few valid positions to spawn berries. Solution might be some whitelist
 * (array) of positions from which this function would random.
 */
Berry spawnBerry(Point from, Point to, Snake snake)
{
    std::mt19937 rng;
    rng.seed(std::random_device()());

    std::uniform_int_distribution<std::mt19937::result_type> distX(from.x + 1, to.x - 1);
    std::uniform_int_distribution<std::mt19937::result_type> distY(from.y + 1, to.y - 1);

    int x = distX(rng);
    int y = distY(rng);

    for (auto coordinate : snake.getCoordinates())
    {
        if (coordinate.x == x && coordinate.y == y)
        {
            return spawnBerry(from, to, snake);
        }
    }

    return Berry(x, y);
}
