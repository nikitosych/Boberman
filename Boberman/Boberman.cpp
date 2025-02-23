#include <algorithm>
#include <iostream>
#include <conio.h>
#include <algorithm>
#include <format> 
#include <random>
#include <vector>
#include <Windows.h>

using namespace std;

// Координаты должны учитывать и нулевую строку и столбец из решеток
int WIDTH = 15; 
int HEIGHT = 15; 
int SQUARE = WIDTH * HEIGHT;

class Events  // Долго объяснять
{
public:
    __event void Boom(int radius);
    __event void DestroyedBlocks(int x, int y);
    __event void Dead();
};

struct Coords
{
    int x;
    int y;
};

static void cpwo(const string& value, int textColor = 7, int bgColor = 0)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole,
        (bgColor << 4) | textColor);

    cout << value;
    SetConsoleTextAttribute(hConsole,
        (0 << 4) | 7);
}

static void cpwo(char c, int textColor = 7, int bgColor = 0)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole,
        (bgColor << 4) | textColor);

    cout << c;
    SetConsoleTextAttribute(hConsole,
        (0 << 4) | 7);
    /*

Color      Background    Foreground
---------------------------------------------
Black            0           0
Blue             1           1
Green            2           2
Cyan             3           3
Red              4           4
Magenta          5           5
Brown            6           6
White            7           7
Gray             -           8
Intense Blue     -           9
Intense Green    -           10
Intense Cyan     -           11
Intense Red      -           12
Intense Magenta  -           13
Yellow           -           14
Intense White    -           15



  */
}

static int decr(int coord) {
    if (max(coord, 1) == 1) return 1; // Шоб не вышшла за границу
    return coord - 1;
}

static int incr(int coord, int border) {
    if (min(coord, border) == border) return border; // Шоб не вышла за границу
    return coord + 1;
}

static vector<Coords> gen_obstacles(int skoka) {
    // я точно не понимаю, как это происходит, но четыре строки кода ниже генерируют случайные значения в нужном диапазоне
    // https://ru.cppreference.com/w/cpp/numeric/random/uniform_int_distribution
    random_device rd; // генератор типа rand()
    mt19937 gen(rd()); // указываем как именно будут подбираться рандомные значения
    uniform_int_distribution<int> dist_x(1, WIDTH - 1);
    uniform_int_distribution<int> dist_y(1, HEIGHT - 1);


    vector<Coords> obstls;
    for (int i = 0; i < skoka; i++) {
        obstls.push_back({ .x= dist_x(gen), .y= dist_y(gen) });
    }

    // Перемешиваем
    shuffle(obstls.begin(), obstls.end(), gen);

    return obstls;
}

static bool collides(const Coords& man, const vector<Coords>& obstacles)
{
    int x = man.x; int y = man.y;
    return find_if(obstacles.begin(), obstacles.end(), [x, y](Coords o) { return o.x == x && o.y == y; }) != obstacles.end();
}


/// <summary>
/// Рысуе поле
/// </summary>
/// <param name="man">человечек, координаты человечка</param>
/// <param name="obstacles">вектор препятствий, который передается по указателю, чтоб его менять внутри функции</param>
static void draw_field(Coords man, const vector<Coords>* obstacles = nullptr)
{
    for (int i = 1; i <= WIDTH + 1; i++) {
        cpwo(" # ", 6, 2);
    } // Первая линия решетками

    cpwo('\n');

    for (int i = 1; i < HEIGHT; i++) {
        cpwo(" # ", 6, 2);
        for (int j = 1; j < WIDTH; j++) {
            if (man.x == j && man.y == i) {
                cpwo(" + ");
            }
            else if (collides({ .x = j, .y = i }, *obstacles)) { // ищем текущие координаты (i, j) в векторе obstacles
                cpwo(" $ ");
            }
            else {
                cpwo("   ");
            }
        }
        cpwo(" # ", 6, 2);
        cpwo('\n');
    }

    for (int i = 1; i <= WIDTH + 1; i++) {
        cpwo(" # ", 6, 2);
    }

    cpwo('\n');
}

static void move(char key, Coords* man, const vector<Coords>& obstacles)
{
    switch (key) {
    case 'w':
        if (collides({ .x= man->x, .y= decr(man->y)}, obstacles)) break;
    	man->y = decr(man->y);
        break;
    case 'a':
        if (collides({ .x= decr(man->x), .y= man->y }, obstacles)) break;
        man->x = decr(man->x);
        break;
    case 's':
        if (collides({.x= man->x, .y= incr(man->y, HEIGHT) }, obstacles)) break;
        man->y = incr(man->y, HEIGHT);
        break;
    case 'd':
        if (collides({ .x = incr(man->x, WIDTH), .y = man->y }, obstacles)) break;
        man->x = incr(man->x, WIDTH);
        break;
    default: break;
    }
}

int main()
{
    Coords man = { .x = 1, .y = 1 };
    (void)setlocale(LC_ALL, "");
    auto obstls = gen_obstacles(SQUARE / 4);

    constexpr int options = 5;
    int curr = 0;

    while (true)
    {
        system("cls");
        cpwo("Welcum to my gayme Boberman! *восьмибитная азартная музяка*\nPress any key to start :)\n\nЧе нада?\n", 3);

        for (int i = 0; i < options; i++)
        {
            char targeted = i == curr ? '+' : ' ';
            switch (i)
            {
            case 0:
                cpwo(format("[ {} ]\t1. Play\n", targeted), 3);
                break;
            case 1:
                cpwo(format("[ {} ]\t2. Change obstacles quantity\n", targeted), 3);
                break;
            case 2:
                cpwo(format("[ {} ]\t3. Print obstacles\n", targeted), 3);
                break;
            case 3:
                cpwo(format("[ {} ]\t4. Change field size\n", targeted), 3);
                break;
            case 4:
                cpwo(format("[ {} ]\t5. Exit\n", targeted), 3);
                break;
            }
        }

        char key = static_cast<char>(_getch());

        if (key == 'w')
        {
            curr = max(0, curr - 1);
        }
        else if (key == 's')
        {
            curr = min(options - 1, curr + 1);
        }
        else if (key == '\r' || key == '\n') // ентер
        {
            if (curr == 0)
                break;
            else if (curr == 1)
            {
                unsigned int obstls_q;
                cpwo(format("Current quantity: {}\nEnter new quantity or pass >SQUARE/4||bullshit to return: ", obstls.size()));
                cin >> obstls_q;
                if (obstls_q <= SQUARE / 4)
                    obstls = gen_obstacles(obstls_q);
            }
            else if (curr == 2)
            {
                cpwo("Current obstacles:\n");
                for (Coords o : obstls)
                {
                    cpwo(format("({}, {}),\n", o.x, o.y));
                }
                cpwo("Почитал? Нажми тогда любую клавищу");
                _getch();
            }
            else if (curr == 3)
            {
                unsigned int width, height;
                cpwo(format("Current width: {}\nCurrent height: {}\nEnter new width or pass >30||bullshit to return: ", WIDTH, HEIGHT));
                cin >> width;
                if (width <= 30)
                    WIDTH = width;
                else continue;
                cpwo("Enter new height or pass >30||bullshit to return: ");
                cin >> height;
                if (height <= 30)
                    HEIGHT = height;
                else continue;
            }
            else if (curr == 4)
            {
                cpwo("Dowidzenia <3\n");
                return 0;
            }
        }
    }

    system("cls");

    while (true)
    {
        if (_kbhit()) // ожидание клавиши в буфере
        {
            char key = static_cast<char>(_getch()); // ожидаем клавишу и явно преобразовываем ее шоб компилятор не гавкал
            system("cls");

            cpwo("Welcum to my gayme Boberman! *восьмибитная азартная музяка*\nPress \"L\" to stop it\n\n", 3);

            if (key == 'l')
            {
                cpwo("Dowidzenia <3\n");
                break;
            }

            move(key, &man, obstls);
            draw_field(man, &obstls);
            cpwo(format("\nКлавиша: {}\n", key), 3);
            cpwo(format("x = {}, y = {}\n", man.x, man.y), 3);
        }
    }
    return 0;
}
