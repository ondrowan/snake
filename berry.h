#pragma once

#include "snake.h"
#include "entity.h"

class Berry : public Entity
{
public:
    Berry(int x, int y);
};

Berry spawnBerry(Point from, Point to, Snake snake);
