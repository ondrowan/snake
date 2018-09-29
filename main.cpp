#include <vector>
#include <chrono>
#include <ncurses.h>
#include "snake.h"
#include "point.h"
#include "berry.h"
#include "world.h"

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

    const Point worldStart{ 0, 0 };
    const Point worldEnd{ 30, 20 };

    std::vector<std::reference_wrapper<Entity>> entities;

    World world(worldStart, worldEnd);
    entities.push_back(world);

    Snake snake(10, 10);
    entities.push_back(snake);

    auto berry = spawnBerry(worldStart, worldEnd, snake);
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
                    berry = spawnBerry(worldStart, worldEnd, snake);
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
        mvprintw(31, 30, "Score: %i", score);

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

        refresh();
    }

    endwin();

    return 0;
}
