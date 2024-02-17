#include <iostream>
#include <ctime>
#include <Windows.h>
using namespace std;

class BombermanGame {
public:

    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    int width;
    int height;
    int** bomber = nullptr;

    BombermanGame(int w, int h) : width(w), height(h) 
    {
        bomber = new int* [height];
        for (int i = 0; i < height; i++) 
        {
            bomber[i] = new int[width];
        }
    }
    //очистка курсора
    void Options()
    {
        HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
        // размер окна - 130х50 символов
        CONSOLE_CURSOR_INFO ci; // скрытие курсора
        ci.bVisible = false; // курсор не видно
        ci.dwSize = 100; // на самом деле, не важно что сюда писать - подходит любое значение от 1 до 100 (рамер курсора в процентах относительно клетки)
        SetConsoleCursorInfo(h, &ci); // применить настройки
    }
    //очистка памяти
    ~BombermanGame() 
    {
        FreeMemory();
    }
    //основные стены
    void Wall()
    {
        for (int y = 0; y < height; y++)
        {
           
            for (int x = 0; x < width; x++)
            {
                bomber[y][x] = rand() % 5;

                if (x == 0 || y == 0 || x == width - 1 || y == height - 1 || x == 1 || x == 2 || y == 1 || x == width - 2 || y == height - 2 || x == width - 3)
                {
                    bomber[y][x] = 1;
                }
                if (x == 0 || y == 0 || x == width - 1 || y == height - 1 || x == 1 || x == 2 || y == 1 || x == width - 2 || y == height - 2 || x == width - 3)
                {
                    SetCursor(x, y, 8);
                    cout << (char)178;
                }
 
            }
        }      
    }
    //закрашиваем координаты
    void SetCursor(int x, int y, int color)
    {
        COORD position;
        position.X = x;
        position.Y = y;
        SetConsoleCursorPosition(h, position);
        SetConsoleTextAttribute(h, color);
    }
    //стены на карте
    void WallNumberTwo(int x, int y) // 2 2
    {
        HANDLE h = GetStdHandle(-11);
        COORD position;
        for (int j = 0; j < 2; j++) // каждая ячейка это две строки по высоте
        {
            for (int i = 0; i < 3; i++) // каждая ячейка это 3 столбика
            {
                position.X = x * 3 + i; // 6 7 8    6 7 8 
                position.Y = y * 2 + j; // 4 4 4    5 5 5
                SetConsoleCursorPosition(h, position);
                SetConsoleTextAttribute(h, 4);
                cout << (char)178;
            }
        }
    }
    //очистка памяти
    void FreeMemory() {
        for (int y = 0; y < height; y++) {
            delete[] bomber[y];
        }
        delete[] bomber;
    }
};
// main
int main()
{
    srand(time(NULL));

    BombermanGame b(63,18);// вызов класса
    b.Options();//скрываем курсор
    b.WallNumberTwo(2,2);
    b.WallNumberTwo(2,4);
    b.WallNumberTwo(2,6);

    b.WallNumberTwo(4,2);
    b.WallNumberTwo(4, 4);
    b.WallNumberTwo(4, 6);

    b.WallNumberTwo(6, 2);
    b.WallNumberTwo(6, 4);
    b.WallNumberTwo(6, 6);

    b.WallNumberTwo(8, 2);
    b.WallNumberTwo(8, 4);
    b.WallNumberTwo(8, 6);

    b.WallNumberTwo(10, 2);
    b.WallNumberTwo(10, 4);
    b.WallNumberTwo(10, 6);

    b.WallNumberTwo(12, 2);
    b.WallNumberTwo(12, 4);
    b.WallNumberTwo(12, 6);

    b.WallNumberTwo(14, 2);
    b.WallNumberTwo(14, 4);
    b.WallNumberTwo(14, 6);

    b.WallNumberTwo(16, 2);
    b.WallNumberTwo(16, 4);
    b.WallNumberTwo(16, 6);

    b.WallNumberTwo(18, 2);
    b.WallNumberTwo(18, 4);
    b.WallNumberTwo(18, 6);
    b.Wall();//основные стны
    cout << "\n";
    
}
