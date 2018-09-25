#pragma once

#include "entity.h"
#include "point.h"

class World : public Entity
{
public:
    World(const Point& from, const Point& to);
};
