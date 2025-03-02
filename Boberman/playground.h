#pragma once

#include <vector>
#include <algorithm>
#include <random>
#include "utils.h"

using namespace std;

struct Coords {
    int x, y;
};

class Playground {
private:
    unsigned int width, height, square, step_counter;

    Coords player;

    struct Wall {
        Coords coords;
        bool isBreakable;
    };
    
    vector<Wall> walls;
public:
    /// <summary>
    ///     »нициализаци€ пол€ с заданными параметрами
    /// </summary>
    /// <param name="startPlayer">Ќачальное положение игрока (рек. 1 и 1)</param>
    /// <param name="width">ƒлина пол€</param>
    /// <param name="height">Ўирина (высота) пол€</param>
    /// <param name="wallsCount"> оличество стен в поле</param>
    Playground(
        Coords startPlayer,
        unsigned int width, 
        unsigned int height,
        unsigned int wallsCount = 0
    ) {
        this->width = width;
        this->height = height;
        this->square = width * height;
        this->player = startPlayer;
        this->step_counter = 0;

        this->gen_walls(wallsCount);
    }
    
// дл€ справки: директивы ниже не вли€ют на компил€цию, 
// а просто позвол€ют кайфово группировать код в visual studio
#pragma region GETTERS
    unsigned int getSteps() {
        return this->step_counter;
    }

    int getX() { return this->player.x; }
    int getY() { return this->player.y; }
    unsigned int getWidth() { return this->width;  }
    unsigned int getHeight() { return this->height; }
    unsigned int getSquare() { return this->square; }
    unsigned int getWallsQ() { return this->walls.size(); }
    const vector<Wall> getWalls() { return this->walls; }

    /// <summary>
    /// »спользует <algorithm> дл€ поиска координат, переданных в параметры, в векторе стен
    /// </summary>
    /// <param name="pos"> акие координаты ищем</param>
    /// <returns>Ќайденна€ стена типа Wall или nullptr если не найдено</returns>
    Wall* findWall(Coords pos) {
        auto iterator = find_if(
            this->walls.begin(),
            this->walls.end(),
            [pos](Wall& w) {
                return w.coords.x == pos.x && w.coords.y == pos.y;
            });
        return (iterator != walls.end()) ? &(*iterator) : nullptr;
    }
    /// <summary>
    /// ѕровер€ет, одинаковы ли координаты игрока и любой стены из вектора walls
    /// </summary>
    /// <returns>Ќайденна€ стена типа Wall, координаты которой совпадают с игроком, или nullptr если не найдено</returns>
    Wall* findWall()
    {
        return findWall({
            .x = this->player.x, 
            .y = this->player.y // через €вное определение свойств избегаем копировани€
            }
        );
    }
#pragma endregion 

#pragma region SETTERS
    void goX(bool goesRight, int border) {
        int* x = &((this->player).x);
        if (goesRight && min(*x, border) == border) {
            *x = border;
        }
        else if (goesRight && min(*x, border) < border) {
            *x += 1;
        }
        else if (!goesRight && max(*x, 1) == 1) {
            *x = 1;
        }
        else if (!goesRight && max(*x, 1) > 1) {
            *x -= 1;
        }

        // return this->getX();
    }

    void goY(bool goesDown, int border) {
        int* y = &((this->player).y);
        if (goesDown && min(*y, border) == border) {
            *y = border;
        }
        else if (goesDown && min(*y, border) < border) {
            *y += 1;
        }
        else if (!goesDown && max(*y, 1) == 1) {
            *y = 1;
        }
        else if (!goesDown && max(*y, 1) > 1) {
            *y -= 1;
        }
        // return this->getY();
    }

    void move(char key)
    {
        Coords* player = &(this->player);

        switch (key) {
        case static_cast<unsigned int>('ж'):
        case 'w':
            if (this->findWall({ .x = player->x, .y = player->y - 1 })) break;
            this->goY(false, this->height);
            ++this->step_counter;
            break;
        case static_cast<unsigned int>('д'):
        case 'a':
            if (this->findWall({ .x = this->player.x - 1, .y = this->player.y })) break;
            goX(false, this->width);
            ++this->step_counter;
            break;
        case static_cast<unsigned int>('л'):
        case 's':
            if (this->findWall({ .x = this->player.x, .y = this->player.y + 1 })) break;
            goY(true, height);
            ++this->step_counter;
            break;
        case static_cast<unsigned int>('Ґ'):
        case 'd':
            if (this->findWall({ .x = this->player.x + 1, .y = this->player.y })) break;
            goX(true, width);
            ++this->step_counter;
            break;
        case '\r':
        case '\n':
            // g_step_counter = 0; //  огда ставим бомбу обнул€м счетчик шагов
            break;
        default: break;
        }
    }
    void gen_walls(int count) {
        // € точно не понимаю, как это происходит, но четыре строки кода ниже генерируют случайные значени€ в нужном диапазоне
        // https://ru.cppreference.com/w/cpp/numeric/random/uniform_int_distribution
        random_device rd; // генератор типа rand()
        mt19937 gen(rd()); // указываем как именно будут подбиратьс€ рандомные значени€
        uniform_int_distribution<int> dist_x(1, this->width - 1);
        uniform_int_distribution<int> dist_y(1, this->height - 1);

        for (int i = 0; i < count; ++i) {
            this->walls.push_back(
                {
                    .coords = {
                        .x = dist_x(gen),
                        .y = dist_y(gen)
                    },
                    .isBreakable = true
                }
            );
        }

        // ѕеремешиваем
        ranges::shuffle(this->walls, gen);
    }
#pragma endregion

    /// <summary>
    /// –ысуе поле
    /// </summary>
    void draw_field()
    {
        for (int i = 1; i <= this->width + 1; ++i) {
            cpwo(" # ", 6, 2);
        } // ѕерва€ лини€ решетками

        cpwo('\n');

        for (int i = 1; i < this->height; ++i) {
            cpwo(" # ", 6, 2);
            for (int j = 1; j < this->width; j++) {
                auto wall = findWall({ .x = j, .y = i }); // ищем текущие координаты (i, j) в векторе walls и получаем, если найдено совпадение
                if (this->player.x == j && this->player.y == i) {
                    cpwo(" + ");
                }
                else if (wall && wall->isBreakable) { 
                    cpwo(" $ ");
                }
                else if (wall && !wall->isBreakable) {
                    cpwo(" # ");
                }
                else {
                    cpwo("   ");
                }
            }
            cpwo(" # ", 6, 2);
            cpwo('\n');
        }

        for (int i = 1; i <= this->width + 1; ++i) {
            cpwo(" # ", 6, 2);
        }

        cpwo('\n');
    }
};