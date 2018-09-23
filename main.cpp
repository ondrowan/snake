#include <cassert>
#include <array>
#include <deque>
#include <vector>
#include <chrono>
#include <ncurses.h>
#include <algorithm>
#include <functional>
#include "snake.h"
#include "point.h"
#include "berry.h"
#include "world.h"

void render(const std::vector<std::reference_wrapper<Entity>>& entities)
{
    for (auto entity : entities)
    {
        const char* character;

        for (auto point : entity.get().getCoordinates())
        {
            switch (point.material)
            {
                case Material::SNAKE_HEAD:
                    character = "O";
                    break;
                case Material::SNAKE_BODY:
                    character = "o";
                    break;
                case Material::WALL:
                    character = "#";
                    break;
                case Material::BERRY:
                    character = ".";
                    break;
            }

            mvprintw(point.y, point.x, character);
        }
    }
}

int main()
{
    int windowHeight = 0, windowWidth = 0;

    auto window = initscr();
    noecho();
    nodelay(window, TRUE);
    curs_set(FALSE);
    getmaxyx(stdscr, windowHeight, windowWidth);

    const Point worldStart{ 0, 0 };
    const Point worldEnd{ 30, 20 };

    std::vector<std::reference_wrapper<Entity>> entities;

    World world(worldStart, worldEnd);
    entities.push_back(world);

    Snake snake(10, 10);
    entities.push_back(snake);

    auto berry = spawnBerry(worldStart, worldEnd);
    entities.push_back(berry);

    auto currentTime = std::chrono::high_resolution_clock::now();
    auto lastMove = currentTime;
    bool gameOver = false;
    int score = 0;

    while (true)
    {
        switch (wgetch(window))
        {
            case 'w':
                snake.setDirection(Direction::UP);
                break;
            case 's':
                snake.setDirection(Direction::DOWN);
                break;
            case 'a':
                snake.setDirection(Direction::LEFT);
                break;
            case 'd':
                snake.setDirection(Direction::RIGHT);
                break;
        }

        auto collision = snake.checkCollision(entities);
        clear();

        if (collision.has_value())
        {
            switch (collision.value().second)
            {
                case Material::WALL:
                    gameOver = true;
                    break;
                case Material::BERRY:
                    score += 1;
                    entities.erase(std::remove_if(entities.begin(), entities.end(), [collision](auto entity) {
                        return std::addressof(entity.get()) == std::addressof(collision.value().first);
                    }));
                    berry = spawnBerry(worldStart, worldEnd);
                    entities.push_back(berry);
                    snake.grow();
                    break;
                case Material::SNAKE_BODY:
                    gameOver = true;
                    break;
                case Material::SNAKE_HEAD:
                    break;
            }
        }
        mvprintw(31, 30, "Score: %i", score);

        if (std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastMove).count() > 200 && !gameOver) {
            snake.move();
            lastMove = std::chrono::high_resolution_clock::now();
        }

        render(entities);
        refresh();

        currentTime = std::chrono::high_resolution_clock::now();
    }

    endwin();

    return 0;
}
