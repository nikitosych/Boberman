#include <iostream>
#include <conio.h>

using namespace std;
int decr(int coord) {
    if (coord <= 1) return 1;
    return coord - 1;
}
int incr(int coord, int border) {
    if (coord == border - 1) return border - 1;
    return coord + 1;
}
int main()
{
    const int width = 11; const int height = 15;
    int x = 1, y = 1;


    while (true) {
        if (_kbhit()) {
            system("cls");
            char key = _getch();

            switch (key) {
            case 'w':
                y = decr(y);
            case 'a':
                x = decr(x);
            case 's':
                y = incr(y, height);
            case 'd':
                x = incr(x, width);
            }

            for (int i = 0; i < height; i++) {
                if (i == 0) {
                    for (int j = 0; j < width; j++) {
                        cout << " # ";
                    }
                    cout << endl;
                }
                else if (i == y) {
                    for (int j = 0; j < width; j++) {
                        if (j == 0) {
                            cout << " # ";
                        }
                        else if (j == x) {
                            cout << " + ";
                        }
                        else cout << "   ";
                    }
                    cout << endl;
                }
                else {
                    for (int j = 0; j < width; j++) {
                        if (j == 0) {
                            cout << " # ";
                        }
                        else cout << "   ";
                    }
                    cout << endl;
                }


            }
            cout << key << endl;
            cout << "x = " << x << ", y = " << y << endl;
        }
    }
}