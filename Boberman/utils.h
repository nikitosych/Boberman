#pragma once

#include <iostream>
#include <format> // требует C++20
#include <Windows.h>

using namespace std;

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

static void cpwo(const string& value, int textColor = 7, int bgColor = 0)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole,
        (bgColor << 4) | textColor);

    cout << value;
    SetConsoleTextAttribute(hConsole,
        (0 << 4) | 7);
}