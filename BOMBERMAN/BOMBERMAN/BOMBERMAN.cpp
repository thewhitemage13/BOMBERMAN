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
        for (int i = 0; i < height; ++i) {
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
    void WallNumberTwo(int x, int y)
    {
        COORD position;
        position.X = x * 3;
        position.Y = y * 2;
        SetCursor(x, y, 12);
        cout << (char)178;
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
    BombermanGame b(60,20);// вызов класса
    b.Options();//скрываем курсор
    b.Wall();//основные стны
    b.WallNumberTwo(6, 5);//стены на карте
    b.WallNumberTwo(6, 6);//стены на карте
    b.WallNumberTwo(7, 6);//стены на карте
    b.WallNumberTwo(8, 6);//стены на карте
    b.WallNumberTwo(7, 5);//стены на карте
    b.WallNumberTwo(8, 5);//стены на карте
    b.WallNumberTwo(0, 0);//стены на карте
}
