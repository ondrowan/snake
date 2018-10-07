#pragma once

enum class Material { WALL, SNAKE_HEAD, SNAKE_BODY, BERRY };

struct Point
{
    int x;
    int y;
    Material material;

    bool operator<(const Point& rhs) const
    {
        return x < rhs.x || y < rhs.y;
    }
};
