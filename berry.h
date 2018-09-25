#pragma once

#include "entity.h"
#include "point.h"

class Berry : public Entity
{
public:
    Berry(int x, int y);
};

Berry spawnBerry(Point from, Point to);
