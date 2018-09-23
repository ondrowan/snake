#include <memory>
#include "snake.h"

Snake::Snake(int x, int y)
{
    std::list<Point> initialCoordinates = {
        Point{ x, y, Material::SNAKE_HEAD },
        Point{ x + 1, y, Material::SNAKE_BODY },
        Point{ x + 2, y, Material::SNAKE_BODY }
    };

    coordinates.insert(coordinates.end(), initialCoordinates.begin(), initialCoordinates.end());
}

void Snake::setDirection(Direction newDirection)
{
    if (
        (lastPerformedDirection == Direction::UP && newDirection == Direction::DOWN) ||
        (lastPerformedDirection == Direction::DOWN && newDirection == Direction::UP) ||
        (lastPerformedDirection == Direction::LEFT && newDirection == Direction::RIGHT) ||
        (lastPerformedDirection == Direction::RIGHT && newDirection == Direction::LEFT)
    )
    {
        return;
    }

    currentDirection = newDirection;
}

void Snake::move()
{
    Point& head = coordinates.front();
    Point tail = coordinates.back();
    Point newHead;

    switch (currentDirection)
    {
        case Direction::UP:
            newHead = Point{ head.x, head.y - 1, Material::SNAKE_HEAD };
            break;
        case Direction::DOWN:
            newHead = Point{ head.x, head.y + 1, Material::SNAKE_HEAD };
            break;
        case Direction::LEFT:
            newHead = Point{ head.x - 1, head.y, Material::SNAKE_HEAD };
            break;
        case Direction::RIGHT:
            newHead = Point{ head.x + 1, head.y, Material::SNAKE_HEAD };
            break;
    }

    head = Point{ head.x, head.y, Material::SNAKE_BODY };

    coordinates.pop_back();
    coordinates.push_front(newHead);

    if (willGrow)
    {
        coordinates.push_back(tail);
        willGrow = false;
    }

    lastPerformedDirection = currentDirection;
}

void Snake::grow()
{
    willGrow = true;
}

std::optional<std::pair<Entity&, Material>> Snake::checkCollision(const std::vector<std::reference_wrapper<Entity>>& entities) const
{
    const Point& head = coordinates.front();

    for (auto entity : entities)
    {
        for (auto point : entity.get().getCoordinates())
        {
            if (head.x == point.x && head.y == point.y && point.material != Material::SNAKE_HEAD)
            {
                return std::make_pair(entity, point.material);
            }
        }
    }

    return {};
}