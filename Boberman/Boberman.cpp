#include <conio.h> 
#include "playground.h" 

// NOTES
// 1. Хранить поле вместе с координатами игрока, препятствий, врагов и бомб как динамические данные (состояния). 
//    Можно имплементировать как классы.
// 2. Использовать ивентовую систему
// 3. Позже сделать игру на winapi, возможно позже и на D3D11.h (оконное, если пойдет по маслу то в трехмерной графике)

using namespace std;

int main()
{
    (void)setlocale(LC_ALL, "");

    Playground field = Playground({ 1, 1 }, 15, 15, (15 * 15) / 3); // Предзагружаем поле для доступа к его свойствам в меню

    constexpr int options = 5;
    int curr = 0;

    while (true)
    {
        system("cls");
        cpwo("Welcum to my gayme Boberman! *восьмибитная азартная музяка*\nPress any key to start :)\n\nЧе нада?\n", 3);

        for (int i = 0; i < options; ++i)
        {
            char targeted = i == curr ? '+' : ' ';
            switch (i)
            {
            case 0:
                cpwo(format("[ {} ]\t\t1. Play\n", targeted), 3);
                break;
            case 1:
                cpwo(format("[ {} ]\t\t2. Change obstacles quantity\n", targeted), 3);
                break;
            case 2:
                cpwo(format("[ {} ]\t\t3. Print obstacles\n", targeted), 3);
                break;
            case 3:
                cpwo(format("[ {} ]\t\t4. Change field size\n", targeted), 3);
                break;
            case 4:
                cpwo(format("[ {} ]\t\t5. Exit\n", targeted), 3);
                break;
            }
        }

        char key = static_cast<char>(_getch());

        unsigned int walls_q = field.getWallsQ();

        if (key == 'w' || key == static_cast<int>('ж'))
        {
            curr--;
            if (curr < 0) curr = options - 1;
        }
        else if (key == 's' || key == static_cast<int>('л'))
        {
            curr++;
            if (curr > options - 1) curr = 0;
        }
        else if (key == '\r' || key == '\n') // ентер
        {
            if (curr == 0)
                break;
            else if (curr == 1)
            {
                cpwo(format("Current quantity: {}\nEnter new quantity or pass >g_square/4||bullshit to return: ", field.getWallsQ()));
                cin >> walls_q;
                if (walls_q <= field.getSquare() / 4)
                     field.gen_walls(walls_q);
            }
            else if (curr == 2)
            {
                cpwo("Current obstacles:\n");
                for (const auto w : field.getWalls())
                {
                    cpwo(format("({}, {}),\n", w.coords.x, w.coords.y));
                }
                cpwo("\nПочитал? Нажми тогда любую клавищу");
                _getch();
            }
            else if (curr == 3)
            {
                unsigned int width, height;
                cpwo(format("Current width: {}\nCurrent height: {}\nEnter new width or pass >30||bullshit to return: ", field.getWidth(), field.getHeight()));
                cin >> width;
                if (width > 30) continue;
                cpwo("Enter new height or pass >30||bullshit to return: ");
                cin >> height;
                if (height > 30) continue;
                field = Playground({ 1,1 }, width, height, walls_q);
            }
            else if (curr == 4)
            {
                cpwo("Dowidzenia <3\n");
                _getch();
                return 0;
            }
        }
    }

    system("cls");

    while (true)
    {
        if (_kbhit()) // проверка нажатой клавиши в буфере консоли
        {
            char key = static_cast<char>(_getch()); // получаем последнюю нажатую клавишу и явно преобразовываем ее шоб редактор не гавкал
            system("cls");

            cpwo("Welcum to my gayme Boberman! *восьмибитная азартная музяка*\nPress \"L\" to stop it\n\n", 3);

            if (key == 'l' || key == '¤')
            {
                cpwo("Dowidzenia <3\n");
                _getch();
                break;
            }

            field.move(key);
            field.draw_field();
            cpwo(format("\nКлавиша: {}({})\n", key, static_cast<int>(key)), 3);
            cpwo(format("x = {}, y = {}\n", field.getX(), field.getY()), 3);
        }
    }
    return 0;
}
