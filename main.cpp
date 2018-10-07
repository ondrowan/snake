#include <vector>
#include <iostream>
#include <chrono>
#include <ncurses.h>
#include <set>
#include <algorithm>
#include "snake.h"
#include "point.h"
#include "berry.h"
#include "world.h"

std::set<Point> getAllCoordinates(const Point& worldStart, const Point& worldEnd)
{
    auto allCoordinates = std::set<Point>{};

    for (int x = worldStart.x + 1; x < worldEnd.x; x++)
    {
        for (int y = worldStart.y + 1; y < worldEnd.y; y++)
        {
            allCoordinates.insert(Point{x, y});
        }
    }

    return allCoordinates;
}

std::vector<Point> getEmptyCoordinates(const std::set<Point>& allCoordinates, const std::vector<std::reference_wrapper<Entity>>& entities)
{
    auto entityCoordinates = std::set<Point>{};

    for (auto entity : entities)
    {
        auto coordinates = entity.get().getCoordinates();
        entityCoordinates.insert(coordinates.begin(), coordinates.end());
    }

    std::vector<Point> emptyCoordinates;
    std::set_difference(allCoordinates.begin(), allCoordinates.end(), entityCoordinates.begin(), entityCoordinates.end(), std::inserter(emptyCoordinates, emptyCoordinates.end()));

    return emptyCoordinates;
}

int main()
{
    int windowHeight = 0, windowWidth = 0;

    auto window = initscr();
    start_color();
    noecho();
    nodelay(window, TRUE);
    curs_set(FALSE);
    getmaxyx(stdscr, windowHeight, windowWidth);

    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);
    init_pair(3, COLOR_WHITE, COLOR_BLACK);
    init_pair(4, COLOR_WHITE, COLOR_RED);

    int worldWidth = 20;
    int worldHeight = 10;

    if (windowWidth < worldWidth || windowHeight < worldHeight + 1) {
        printf("Terminal window is too small to play the game.\n");
        return 0;
    }

    int worldStartX = windowWidth / 2 - worldWidth / 2;
    int worldStartY = windowHeight / 2 - worldWidth / 2;
    int worldEndX = worldStartX + worldWidth;
    int worldEndY = worldStartY + worldHeight;

    const Point worldStart{ worldStartX, worldStartY };
    const Point worldEnd{ worldEndX, worldEndY };
    auto allCoordinates = getAllCoordinates(worldStart, worldEnd);

    std::vector<std::reference_wrapper<Entity>> entities;

    World world(worldStart, worldEnd);
    entities.push_back(world);

    Snake snake(worldStartX + 5, worldStartY + 5);
    entities.push_back(snake);

    auto berry = spawnBerry(getEmptyCoordinates(allCoordinates, entities));
    entities.push_back(berry);

    auto lastMove = std::chrono::high_resolution_clock::now();
    bool gameOver = false;
    int score = 0;

    while (true)
    {
        auto currentTime = std::chrono::high_resolution_clock::now();
        auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastMove).count();

        // Handle input

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

        // Update

        if (elapsedTime > 200 && !gameOver) {
            snake.move();
            lastMove = std::chrono::high_resolution_clock::now();
        }

        auto collision = snake.checkCollision(entities);

        if (collision.has_value())
        {
            switch (collision.value().second)
            {
                case Material::WALL:
                case Material::SNAKE_BODY:
                    gameOver = true;
                    break;
                case Material::BERRY:
                    score += 1;
                    entities.erase(std::remove_if(entities.begin(), entities.end(), [collision](auto entity) {
                        return std::addressof(entity.get()) == std::addressof(collision.value().first);
                    }));
                    berry = spawnBerry(getEmptyCoordinates(allCoordinates, entities));
                    entities.push_back(berry);
                    snake.grow();
                    break;
                case Material::SNAKE_HEAD:
                    break;
            }
        }

        // Render

        clear();

        attron(COLOR_PAIR(3));
        mvprintw(worldEndY + 1, worldEndX - worldWidth, "Score: %i", score);

        for (auto entity : entities)
        {
            const char* character;

            for (auto point : entity.get().getCoordinates())
            {
                switch (point.material)
                {
                    case Material::SNAKE_HEAD:
                        attron(COLOR_PAIR(1));
                        character = "O";
                        break;
                    case Material::SNAKE_BODY:
                        attron(COLOR_PAIR(1));
                        character = "o";
                        break;
                    case Material::WALL:
                        attron(COLOR_PAIR(3));
                        character = "#";
                        break;
                    case Material::BERRY:
                        attron(COLOR_PAIR(2));
                        character = ".";
                        break;
                }

                mvprintw(point.y, point.x, character);
            }
        }

        if (gameOver) {
            attron(COLOR_PAIR(4));
            mvprintw(worldEndY - worldHeight / 2, worldEndX - worldWidth / 2 - 5, "GAME OVER");
        }

        refresh();
    }

    endwin();

    return 0;
}
