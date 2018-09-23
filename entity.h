#pragma once

#include <list>
#include "point.h"

class Entity
{
public:
    const std::list<Point>& getCoordinates() const
    {
        return coordinates;
    }

protected:
    std::list<Point> coordinates;
};