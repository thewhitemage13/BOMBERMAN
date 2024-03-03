#include <iostream>
#include <ctime>
#include <Windows.h>
#include <conio.h> 
using namespace std;
enum MazeObject { HALL = 0, WALL = 1, WALLTWO = 3, ENEMY = 4};
enum Color { DARKGREEN = 2, YELLOW = 14, RED = 12, BLUE = 9, WHITE = 15, DARKYELLOW = 6, DARKRED = 4, PURPUR = 13, GREEN = 10 };
enum KeyCode { ENTER = 13, ESCAPE = 27, SPACE = 32, LEFT = 75, RIGHT = 77, UP = 72, DOWN = 80 };

class BombermanGame {
public:

    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    int width;
    int height;
    int** bomber = nullptr;
    
    void Generation()//сделал генерацию для массива bomber
    {
        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                bomber[y][x] = rand() % 5;
            }
        }
    }

    BombermanGame(int w, int h) : width(w), height(h)
    {

        bomber = new int* [height];
        for (int i = 0; i < height; i++)
        {
            bomber[i] = new int[width];
        }
        Generation();//метод находится в КСП для того чтоб генерация была доступна для всех мктодов класса
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
   
    void Wall()
    {
        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {     
                if (x == 0 || y == 0 || x == width - 1 || y == height - 1)
                {
                    bomber[y][x] = WALL;
                    SetCursor(x, y, 8);
                    cout << (char)178;
                }
            }
        }
    }
    void Enemy()
    {
        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                int r = rand() % 10; // 0...9
                // если выпадает 0 - враг остаётся (10% случаев)
                if (r != 0) // если выпадает не 0 (а число от 1 до 9) - врага убираем (вероятность 90%)
                {
                    bomber[y][x] = 0; // убираем врага
                }
                if (bomber[y][x] == ENEMY)
                {
                    SetCursor(x, y, RED);
                    cout << (char)224;
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
            if (code == RIGHT && bomber[position.Y][position.X + 1] != WALL) // right
            {
                    position.X++; // изменение позиции ГГ вправо на 1 по иксу
            }
            else if (code == LEFT && bomber[position.Y][position.X - 1] != WALL)
            {
                    position.X--;// изменение позиции ГГ влево на 1 по иксу            
            }
            else if (code == UP && bomber[position.Y - 1][position.X] != WALL)
            {              
                    position.Y--;// изменение позиции ГГ вверх на 1 по иксу            
            }
            else if (code == DOWN && bomber[position.Y + 1][position.X] != WALL)
            {
                    position.Y++;// изменение позиции ГГ вниз на 1 по иксу
            }
            // Если нажата клавиша пробела
            else if (code == SPACE && bomber[position.Y][position.X + 1] != WALL)
            {
                    // Рисуем "бомбу" перед персонажем
                    SetCursor(position.X + 1, position.Y, PURPUR);
                    cout << (char)254;
            }
            SetCursor(position.X, position.Y, 12);
            cout << (char)1;
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
    void WallNumberTwo(int x, int y) 
    {
        HANDLE h = GetStdHandle(-11);
        COORD position;
        for (int j = 0; j < 1; j++)
        {
            for (int i = 0; i < 1; i++) {
                position.X = x * 2 + i;
                position.Y = y * 1 + j;
                bomber[position.Y][position.X] = WALLTWO;
                SetConsoleCursorPosition(h, position);
                SetConsoleTextAttribute(h, 7);
                cout << (char)178;
            }
        }
    }

    void AutomateWallNumberTwo()
    {
        for (int i = 2; i <= 28; i += 2) // идем по каждой второй строке
        {
            for (int j = 2; j <= 14; j += 2) // идем по каждой второй колонке
            {
                WallNumberTwo(i, j); // вызываем метод вывода стены с номером два
            }
        }
    }
    //очистка памяти
    void FreeMemory() 
    {
        for (int y = 0; y < height; y++) 
        {
            delete[] bomber[y];
        }
        delete[] bomber;
    }
};
// main
int main()
{
    srand(time(NULL));
    BombermanGame b(61, 17);// вызов класса

    b.Options();
  
    b.Enemy();
    b.Wall();
    b.AutomateWallNumberTwo();
    b.Person(2,2);
    
    //b.Person(2, 2);  
    //int code = _getch();
    //cout << code << "\n";
}
