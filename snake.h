#pragma once

#include <list>
#include <vector>
#include <utility>
#include "optional.h"
#include "entity.h"
#include "point.h"

enum class Direction { UP, DOWN, RIGHT, LEFT };

class Snake : public Entity
{
public:
    Snake(int x, int y);

    void setDirection(Direction newDirection);
    void move();
    void grow();
    std::optional<std::pair<Entity&, Material>> checkCollision(const std::vector<std::reference_wrapper<Entity>>& entities) const;

private:
    Direction lastPerformedDirection;
    Direction currentDirection = Direction::LEFT;
    bool willGrow = false;
};