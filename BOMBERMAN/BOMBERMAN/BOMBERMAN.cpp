#include <iostream>
#include <ctime>
#include <Windows.h>
using namespace std;

class BombermanGame {
public:

    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    int width;
    int height;
    int** bomber = new int* [width];

    BombermanGame(int w, int h) : width(w), height(h) {

        bomber = new int* [width];
        for (int i = 0; i < width; ++i) {
            bomber[i] = new int[height];
        }
    }

    ~BombermanGame() 
    {
        FreeMemory();
    }

    void Wall()
    {
        for (int y = 0; y < height; y++)
        {
            bomber[y] = new int[height];

            for (int x = 0; x < width; x++)
            {
                bomber[y][x] = rand() % 5;

                if (x == 0 || y == 0 || x == width - 1 || y == height - 1 || x == 1 || x == 2 || y == 1 || x == width - 2 || y == height - 2 || x == width - 3)
                {
                    bomber[y][x] = 1;
                }
                if (x == 0 || y == 0 || x == width - 1 || y == height - 1 || x == 1 || x == 2 || y == 1 || x == width - 2 || y == height - 2 || x == width - 3)//стена
                {
                    SetCursor(x, y, 8, "");
                    cout << (char)178;
                }


            }
        }      
    }

    void SetCursor(int x, int y, int color, string text)
    {
        COORD position;
        position.X = x;
        position.Y = y;
        SetConsoleCursorPosition(h, position);
        SetConsoleTextAttribute(h, color);
        cout << text;
    }

    void FreeMemory() {
        for (int y = 0; y < height; y++) {
            delete[] bomber[y];
        }
        delete[] bomber;
    }

};

int main()
{
    srand(time(NULL));
    BombermanGame b(60,20);
    b.Wall();
    b.FreeMemory();

}