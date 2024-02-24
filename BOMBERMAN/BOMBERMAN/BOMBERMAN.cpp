#include <iostream>
#include <ctime>
#include <Windows.h>
#include <conio.h> 
using namespace std;

class BombermanGame {
public:
    enum MazeObject { HALL, WALL, COIN, ENEMY, BORDER };
    enum Color { DARKGREEN = 2, YELLOW = 14, RED = 12, BLUE = 9, WHITE = 15, DARKYELLOW = 6, DARKRED = 4, PURPUR = 13, GREEN = 10 };
    enum KeyCode { ENTER = 13, ESCAPE = 27, SPACE = 32, LEFT = 75, RIGHT = 77, UP = 72, DOWN = 80 };


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
    void Person(int x, int y) // Рисует персонажа в указанных координатах
    {
        COORD position;
        position.X = x;
        position.Y = y;
        SetCursor(x, y, 12);
        cout << (char)1; // Можно использовать любой другой символ или строку для отображения персонажа

        while (true)
        {
            int code = _getch(); // Получаем код нажатой клавиши
            if (code == 224) // Если нажата клавиша стрелки
            {
                code = _getch(); // Получаем расширенный код клавиши
            }

            // Стираем персонажа в текущей позиции
            SetConsoleCursorPosition(h, position);
            cout << " ";

            if (code == RIGHT && bomber[position.Y][position.X + 1] != 1) // right
            {
                // Проверяем, есть ли стена справа
                if (bomber[position.Y][position.X + 1] != 1)
                {
                    position.X++; // изменение позиции ГГ вправо на 1 по иксу
                }
            }
            else if (code == LEFT && bomber[position.Y][position.X - 1] != 1)
            {
                // Проверяем, есть ли стена слева
                if (bomber[position.Y][position.X - 1] != 1)
                {
                    position.X--;// изменение позиции ГГ влево на 1 по иксу
                }
            }
            else if (code == UP && bomber[position.Y - 1][position.X] != 1)
            {
                // Проверяем, есть ли стена сверху
                if (bomber[position.Y - 1][position.X] != 1)
                {
                    position.Y--;// изменение позиции ГГ вверх на 1 по иксу
                }
            }
            else if (code == DOWN && bomber[position.Y + 1][position.X] != 1)
            {
                // Проверяем, есть ли стена снизу
                if (bomber[position.Y + 1][position.X] != 1)
                {
                    position.Y++;// изменение позиции ГГ вниз на 1 по иксу
                }
            }
            else if (code == SPACE) { // Если нажата клавиша пробела
                // Проверяем, что перед персонажем нет стены
                if (bomber[position.Y][position.X + 1] != 1) {
                    // Рисуем "бомбу" перед персонажем
                    SetCursor(position.X + 1, position.Y, PURPUR);
                    cout << (char)254;
                }
            }
                // Рисуем персонажа в новой позиции
                SetCursor(position.X, position.Y, 12);
                cout << (char)1;
            
            // Опционально можно добавить проверку на нажатие клавиши выхода, например ESCAPE

        }
    }

    void KeyBoard(int x, int y)
    {
        COORD position;
        position.X = x;
        position.Y = y;
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
                SetConsoleTextAttribute(h, 8);
                cout << (char)178;
            }
        }
    }

    void AutomateWallNumberTwo()
    {
        for (int i = 2; i <= 18; i += 2) // идем по каждой второй строке
        {
            for (int j = 2; j <= 8; j += 2) // идем по каждой второй колонке
            {
                WallNumberTwo(i, j); // вызываем метод вывода стены с номером два
            }
        }
    }

    void WallNumberTThree(int x, int y) // 2 2
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
    BombermanGame b(63, 18);// вызов класса
    b.Options();//скрываем курсор
    b.AutomateWallNumberTwo();
    b.Wall();//основные стны
    b.Person(3, 3);
}
