#include <iostream>
#include <ctime>
#include <Windows.h>
#include <conio.h> 
#include <SFML/Audio.hpp>
using namespace std;

enum MazeObject { HALL = 0, WALL = 1, WALLTWO = 2, WALLTHREE = 3, ENEMY = 4, BOMB = 5, HEALTH = 6 };
enum Color { DARKGREEN = 2, YELLOW = 14, RED = 12, BLUE = 9, WHITE = 15, DARKYELLOW = 6, DARKRED = 4, PURPUR = 13, GREEN = 10 };
enum KeyCode { ENTER = 13, ESCAPE = 27, SPACE = 32, LEFT = 75, RIGHT = 77, UP = 72, DOWN = 80 };

HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

void Help();
void SetCursor();
void Text_effect();
void KeyBoard();
void Stand();
void Options();
void The_end();

void PlayS(string text, int volume) {
    sf::SoundBuffer BlusterV1Buffer; BlusterV1Buffer.loadFromFile(text);
    sf::Sound BlasterV1(BlusterV1Buffer);
    BlasterV1.setVolume(volume);
}

void SetCursor(int x, int y, int color) {
    COORD position;
    position.X = x;
    position.Y = y;
    SetConsoleCursorPosition(h, position);
    SetConsoleTextAttribute(h, color);
}

void Text_effect(int x, int y, int forecolor, int backcolor, const char* text, int len, int pause)
{
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    for (int i = 0; i < len; i++)
    {
        COORD temp = { x + i, y }; // будем ставить курсор в определённые координаты
        SetConsoleCursorPosition(h, temp); // ставим
        SetConsoleTextAttribute(h, backcolor * 16 + forecolor); // применяем цвет
        cout << text[i]; // рисуем букву
        Sleep(pause); // ждём
    }
}

void KeyBoard(int x, int y) {
    COORD position;
    position.X = x;
    position.Y = y;
}

void Stand(int x, int y, int k, const char* str)
{
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD c{ x, y };
    SetConsoleCursorPosition(h, c);
    SetConsoleTextAttribute(h, k);
    cout << str << "\n";
}

void Options() {
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    // размер окна - 130х50 символов
    CONSOLE_CURSOR_INFO ci; // скрытие курсора
    ci.bVisible = false; // курсор не видно
    ci.dwSize = 100; // на самом деле, не важно что сюда писать - подходит любое значение от 1 до 100 (рамер курсора в процентах относительно клетки)
    SetConsoleCursorInfo(h, &ci); // применить настройки
}

class Maze {
private:
    int width;
    int height;
    int** bomber = nullptr;
public:

    Maze() : Maze(61, 17) {

    }

    Maze(const Maze& original) {
        width = original.width;
        height = original.height;

        if (original.bomber != nullptr) {
            bomber = new int* [height];
            for (int i = 0; i < height; i++) {
                bomber[i] = new int[width];
            }

            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    bomber[y][x] = original.bomber[y][x];
                }
            }
        }
    }

    int** GetBomber() const {
        return bomber;
    }

    HANDLE GetH() {
        return h;
    }

    void Generation() { //сделал генерацию для массива bomber
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                bomber[y][x] = rand() % 7;
                if (x == 0 || x == width - 1 || y == 0 || y == height - 1)
                    bomber[y][x] = WALL;
            }
        }
    }

    Maze(int w, int h) : width(w), height(h) {
        bomber = new int* [height];
        for (int i = 0; i < height; i++) {
            bomber[i] = new int[width];
        }
        Generation(); //метод находится в КСП для того чтоб генерация была доступна для всех мктодов класса
    }

    void SetWidth(int width) {
        this->width = width;
    }

    int GetWidth() {
        return width;
    }

    void SetHeight(int height) {
        this->height = height;
    }

    int GetHeight() {
        return height;
    }

    ~Maze() {
        if (bomber != nullptr) {
            for (int y = 0; y < height; y++) {
                delete[] bomber[y];
            }
            delete[] bomber;
        }
    }
};

class Enemy {
private:
    Maze* maze;
public:

    Enemy(Maze* m) {
        if (m != nullptr) {
            maze = m; // Создание нового объекта Maze путем копирования
        }
    }

    void EnemyGeneration() {
        int** bomber = maze->GetBomber();

        for (int y = 0; y < maze->GetHeight(); y++) {
            for (int x = 0; x < maze->GetWidth(); x++) {
                int r = rand() % 5; // 0...9
                // если выпадает 0 - враг остаётся (10% случаев)
                if (r != 0) {// если выпадает не 0 (а число от 1 до 9) - врага убираем (вероятность 90%)
                    bomber[y][x] = 0; // убираем врага
                }
                if (bomber[y][x] == ENEMY) {
                    SetCursor(x, y, RED);
                    cout << (char)224;
                }
            }
        }
    }

    void Joystick() {

        Sleep(15);
        // движение врагов
        COORD enemy_positions[100]; // массив который хранит координаты врагов
        int enemy_count = 0; // реальное количество врагов на данный момент после шага ГГ
        // перебор всех ячеек которые есть в лабиринте
        for (int y = 0; y < maze->GetHeight(); y++) {// перебор строк
            for (int x = 0; x < maze->GetWidth(); x++) { // перебор столбцов
                // если очередная ячейка - это враг
                if (maze->GetBomber()[y][x] == MazeObject::ENEMY) {
                    enemy_positions[enemy_count].X = x;
                    enemy_positions[enemy_count].Y = y;
                    enemy_count++;
                }
            }
        }
        // перебор всех врагов
        for (int i = 0; i < enemy_count; i++) {
            int r = rand() % 100; // 0-left, 1-right, 2-up, 3-down
            int x = enemy_positions[i].X;
            int y = enemy_positions[i].Y;
            if (r == LEFT && x > 1
                && maze->GetBomber()[y][x - 1] != MazeObject::WALL
                && maze->GetBomber()[y][x - 1] != MazeObject::WALLTWO
                && maze->GetBomber()[y][x - 1] != MazeObject::ENEMY
                && maze->GetBomber()[y][x - 1] != MazeObject::BOMB
                && maze->GetBomber()[y][x - 1] != MazeObject::HEALTH
                && maze->GetBomber()[y][x - 1] != MazeObject::WALLTHREE
                ) {
                // стирание врага в старой позиции
                COORD temp = enemy_positions[i];
                SetConsoleCursorPosition(maze->GetH(), temp);
                cout << " ";
                maze->GetBomber()[enemy_positions[i].Y][enemy_positions[i].X] = MazeObject::HALL;
                // перемещаем врага в новую позицию
                temp.X = enemy_positions[i].X - 1;
                temp.Y = enemy_positions[i].Y;
                SetConsoleCursorPosition(maze->GetH(), temp);
                SetConsoleTextAttribute(maze->GetH(), Color::RED);
                cout << (char)224;
                maze->GetBomber()[enemy_positions[i].Y][enemy_positions[i].X - 1] = MazeObject::ENEMY;
            }


            else if (r == RIGHT && x < maze->GetWidth() - 2 &&
                maze->GetBomber()[y][x + 1] != MazeObject::WALL &&
                maze->GetBomber()[y][x + 1] != MazeObject::WALLTWO &&
                maze->GetBomber()[y][x + 1] != MazeObject::ENEMY &&
                maze->GetBomber()[y][x + 1] != MazeObject::BOMB &&
                maze->GetBomber()[y][x + 1] != MazeObject::HEALTH &&
                maze->GetBomber()[y][x + 1] != MazeObject::WALLTHREE
                ) {

                COORD temp = enemy_positions[i];
                SetConsoleCursorPosition(maze->GetH(), temp);
                cout << " ";
                maze->GetBomber()[enemy_positions[i].Y][enemy_positions[i].X] = MazeObject::HALL;

                temp.X = enemy_positions[i].X + 1;
                temp.Y = enemy_positions[i].Y;
                SetConsoleCursorPosition(maze->GetH(), temp);
                SetConsoleTextAttribute(maze->GetH(), Color::RED);
                cout << (char)224;
                maze->GetBomber()[enemy_positions[i].Y][enemy_positions[i].X + 1] = MazeObject::ENEMY;
            }


            else if (r == UP && y > 1 &&
                maze->GetBomber()[y - 1][x] != MazeObject::WALL &&
                maze->GetBomber()[y - 1][x] != MazeObject::WALLTWO &&
                maze->GetBomber()[y - 1][x] != MazeObject::ENEMY &&
                maze->GetBomber()[y - 1][x] != MazeObject::BOMB &&
                maze->GetBomber()[y - 1][x] != MazeObject::HEALTH &&
                maze->GetBomber()[y - 1][x] != MazeObject::WALLTHREE
                ) {

                COORD temp = enemy_positions[i];
                SetConsoleCursorPosition(maze->GetH(), temp);
                cout << " ";
                maze->GetBomber()[enemy_positions[i].Y][enemy_positions[i].X] = MazeObject::HALL;

                temp.X = enemy_positions[i].X;
                temp.Y = enemy_positions[i].Y - 1;
                SetConsoleCursorPosition(maze->GetH(), temp);
                SetConsoleTextAttribute(maze->GetH(), Color::RED);
                cout << (char)224;
                maze->GetBomber()[enemy_positions[i].Y - 1][enemy_positions[i].X] = MazeObject::ENEMY;
            }

            else if (r == DOWN && y < maze->GetHeight() - 2 &&
                maze->GetBomber()[y + 1][x] != MazeObject::WALL &&
                maze->GetBomber()[y + 1][x] != MazeObject::WALLTWO &&
                maze->GetBomber()[y + 1][x] != MazeObject::ENEMY &&
                maze->GetBomber()[y + 1][x] != MazeObject::BOMB &&
                maze->GetBomber()[y + 1][x] != MazeObject::HEALTH &&
                maze->GetBomber()[y + 1][x] != MazeObject::WALLTHREE
                ) {

                COORD temp = enemy_positions[i];
                SetConsoleCursorPosition(maze->GetH(), temp);
                cout << " ";
                maze->GetBomber()[enemy_positions[i].Y][enemy_positions[i].X] = MazeObject::HALL;

                temp.X = enemy_positions[i].X;
                temp.Y = enemy_positions[i].Y + 1;
                SetConsoleCursorPosition(maze->GetH(), temp);
                SetConsoleTextAttribute(maze->GetH(), Color::RED);
                cout << (char)224;
                maze->GetBomber()[enemy_positions[i].Y + 1][enemy_positions[i].X] = MazeObject::ENEMY;
            }
        }
    }
};

class Bomb {
    int count_of_bombs;
    int using_bombs = 0;
    bool bomb;
    int bX;
    int bY;

public:
    Bomb() {
        SetCountOfBomb(10);
        SetUsingBombs(0);
    }

    void SetCountOfBomb(int count_of_bombs) {
        this->count_of_bombs = count_of_bombs;
    }

    void SetUsingBombs(int using_bombs) {
        this->using_bombs = using_bombs;
    }

    void SetMinusBomb(int count_of_bombs) {
        this->count_of_bombs -= count_of_bombs;
    }

    void SetPlusBomb(int count_of_bombs) {
        this->count_of_bombs += count_of_bombs;
    }

    int GetCountOfBomb() {
        return count_of_bombs;
    }

    int GetUsingBombs() {
        return using_bombs;
    }

    void SetBomb(bool bomb) {
        this->bomb = bomb;
    }

    bool GetBomb() {
        return bomb;
    }

    void SetbX(int bX) {
        this->bX = bX;
    }

    int GetbX() {
        return bX;
    }

    void SetbY(int bY) {
        this->bY = bY;
    }

    int GetbY() {
        return bY;
    }
};

class Bomber {
private:
    Bomb* bomb;
    Maze* maze;
    Enemy* enemy;
    int health_person = 100;
    int count_of_coins = 0;
public:
    
    Bomber(Maze* m, Bomb* b, Enemy* e) : maze(m), bomb(b),  enemy(e) {
        SetHealthPerson(100);
        SetCountOfCoins(0);
    }
    
    Bomber(const Bomber& original) {
        health_person = original.health_person;
        count_of_coins = original.count_of_coins;
        this->bomb = new Bomb(*original.bomb);
        this->enemy = new Enemy(*original.enemy);
        this->maze = new Maze(*original.maze);
    }

    Bomber() {
        SetHealthPerson(100);
        SetCountOfCoins(0);
    }

    void SetHealthPerson(int health_person) {
        this->health_person = health_person;
    }

    void SetCountOfCoins(int count_of_coins) {
        this->count_of_coins = count_of_coins;
    }

    int GetHealthPerson() {
        return health_person;
    }

    int GetCountOfCoins() {
        return count_of_coins;
    }

    void Person(int x, int y) {// Рисует персонажа в указанных координатах

        if (maze == nullptr || bomb == nullptr || enemy == nullptr) {
            // Обработка ошибки или выход из метода
            cout << "ERRRRRROOORRRRRR\n";
            return;
        }


        COORD position;
        position.X = x;
        position.Y = y;
        
        SetCursor(x, y, 12);
        cout << (char)1; // Можно использовать любой другой символ или строку для отображения персонажа

        while (true) {
            if (_kbhit()) {//нажатие было
                int code = _getch(); // Получаем код нажатой клавиши
                if (code == 224) {// Если нажата клавиша стрелки
                    code = _getch(); // Получаем расширенный код клавиши
                }
                // Стираем персонажа в текущей позиции
                SetConsoleCursorPosition(maze->GetH(), position);
                cout << " ";
                if (code == RIGHT && maze->GetBomber()[position.Y][position.X + 1] != WALL && maze->GetBomber()[position.Y][position.X + 1] != WALLTWO && maze->GetBomber()[position.Y][position.X + 1] != WALLTHREE) {// right 
                    PlayS("C:\\Users\\lolim\\Documents\\MusicBomberMan\\person.wav", 20);
                    position.X++; // изменение позиции ГГ вправо на 1 по иксу
                }
                else if (code == LEFT && maze->GetBomber()[position.Y][position.X - 1] != WALL && maze->GetBomber()[position.Y][position.X - 1] != WALLTWO && maze->GetBomber()[position.Y][position.X - 1] != WALLTHREE) {
                    PlayS("C:\\Users\\lolim\\Documents\\MusicBomberMan\\person.wav", 20);
                    position.X--;// изменение позиции ГГ влево на 1 по иксу            
                }
                else if (code == UP && maze->GetBomber()[position.Y - 1][position.X] != WALL && maze->GetBomber()[position.Y - 1][position.X] != WALLTWO && maze->GetBomber()[position.Y - 1][position.X] != WALLTHREE) {
                    PlayS("C:\\Users\\lolim\\Documents\\MusicBomberMan\\person.wav", 20);
                    position.Y--;// изменение позиции ГГ вверх на 1 по иксу          
                }
                else if (code == DOWN && maze->GetBomber()[position.Y + 1][position.X] != WALL && maze->GetBomber()[position.Y + 1][position.X] != WALLTWO && maze->GetBomber()[position.Y + 1][position.X] != WALLTHREE) {
                    PlayS("C:\\Users\\lolim\\Documents\\MusicBomberMan\\person.wav", 20);
                    position.Y++;// изменение позиции ГГ вниз на 1 по иксу
                }
                // Если нажата клавиша пробела
                else if (code == SPACE && maze->GetBomber()[position.Y][position.X + 1] != WALL && bomb->GetCountOfBomb() != 0) {
                    bomb->SetBomb(true);
                    bomb->SetbX(position.X);
                    bomb->SetbY(position.Y);
                }

                else if (code == 13 && bomb->GetCountOfBomb() != 0) {
                    if (bomb->GetBomb() == true) {
                        PlayS("C:\\Users\\lolim\\Documents\\MusicBomberMan\\pushbomb.wav", 35);
                        SetCursor(bomb->GetbX(), bomb->GetbY(), PURPUR);
                        cout << (char)254;
                    }
                }

                else if (code == 103 && bomb->GetCountOfBomb() != 0) {
                    if (bomb->GetBomb() == true) {
                        PlayS("C:\\Users\\lolim\\Documents\\MusicBomberMan\\bomb.wav", 35);
                        bomb->SetMinusBomb(1);
                        COORD bomb_position;
                        bomb_position.X = bomb->GetbX();
                        bomb_position.Y = bomb->GetbY();

                        for (int i = -1; i <= 1; ++i) {
                            for (int j = -1; j <= 1; ++j) {
                                int newX = bomb_position.X + i;
                                int newY = bomb_position.Y + j;

                                if (newX >= 0 && newX < maze->GetWidth() && newY >= 0 && newY < maze->GetHeight()) {
                                    if (maze->GetBomber()[newY][newX] != WALL) {
                                        int a = rand() % 2 + 5;
                                        if (maze->GetBomber()[newY][newX] == WALLTHREE) {
                                            if (a == 5) {
                                                maze->GetBomber()[newY][newX] = BOMB;
                                                SetCursor(newX, newY, PURPUR);
                                                cout << (char)254;
                                            }

                                            if (a == 6) {
                                                maze->GetBomber()[newY][newX] = HEALTH;
                                                SetCursor(newX, newY, 14);
                                                cout << (char)3;
                                            }
                                        }
                                        else {
                                            maze->GetBomber()[newY][newX] = 0;
                                            SetCursor(newX, newY, 0);
                                            cout << ' ';
                                        }
                                    }
                                }
                            }
                        }
                    }
                }

                SetCursor(position.X, position.Y, 12);
                cout << (char)1;

                if (position.X == maze->GetWidth() - 2 and position.Y == maze->GetHeight() - 2) {
                    MessageBoxA(0, "You found your way out!!!!", "You win!!!", MB_OK);
                    break;
                }

                SetCursor(maze->GetWidth() + 1, 1, PURPUR);
                cout << "Hit points: ";
                cout << health_person;

                if (maze->GetBomber()[position.Y][position.X] == ENEMY) {
                    health_person -= 20;
                    maze->GetBomber()[position.Y][position.X] = 0;
                    SetCursor(maze->GetWidth() + 1, 1, PURPUR);
                    cout << "Hit points: ";
                    cout << health_person;
                    cout << " ";
                }

                if (maze->GetBomber()[position.Y][position.X] == HEALTH and health_person < 100) {
                    health_person += 20;
                    maze->GetBomber()[position.Y][position.X] = 0;
                    SetCursor(maze->GetWidth() + 1, 1, PURPUR);
                    cout << "Hit points: ";
                    cout << health_person;
                    cout << " ";
                }

                SetCursor(maze->GetWidth() + 1, 3, YELLOW);
                cout << "Number of bombs: ";
                cout << bomb->GetCountOfBomb();
                cout << " ";

                if (code == 103) {
                    SetCursor(maze->GetWidth() + 1, 3, YELLOW);
                    cout << "Number of bombs: ";
                    cout << bomb->GetCountOfBomb();
                    cout << " ";
                }

                if (maze->GetBomber()[position.Y][position.X] == BOMB and bomb->GetCountOfBomb() < 10) {
                    bomb->SetPlusBomb(1);
                    
                    maze->GetBomber()[position.Y][position.X] = 0;
                    SetCursor(maze->GetWidth() + 1, 3, YELLOW);
                    cout << "Number of bombs: ";
                    cout << bomb->GetCountOfBomb();
                    cout << " ";
                }

                if (health_person == 0) {
                    MessageBoxA(0, "You have run out of health!!!!", "You are dead!!!!", MB_OK);
                    break;
                }
            }

            else { // нажатия не было, двигаем врагов
                enemy->Joystick();
            }
        }
    }

    ~Bomber() {
        if (enemy != nullptr) {
            delete enemy;
        }
        if (bomb != nullptr) {
            delete bomb;
        }
    }
};

class Wall {
    Maze* maze = nullptr;
public:

    Wall(Maze* m) {
        if (m != nullptr) {
            maze = m; // Создание нового объекта Maze путем копирования
        }
    }

    Wall(const Wall& original) {
        this->maze = original.maze;
    }

    void WallGenerate() {
        int** bomber = maze->GetBomber();
        for (int y = 0; y < maze->GetHeight(); y++) {
            for (int x = 0; x < maze->GetWidth(); x++) {
                if (x == 0 || y == 0 || x == maze->GetWidth() - 1 || y == maze->GetHeight() - 1) {
                    SetCursor(x, y, 8);
                    cout << (char)178;
                }
                if (bomber[y][x] == 0) {//коридор
                    SetCursor(x, y, 0);
                }
            }
        }
    }

    void WallsInsideTheMapOne(int x, int y) {
        int** bomber = maze->GetBomber();
        HANDLE h = GetStdHandle(-11);
        COORD position;
        for (int j = 0; j < 1; j++) {
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

    void AutomateWallNumberTwo() {
        for (int i = 2; i <= 28; i += 2) {// идем по каждой второй строке
            for (int j = 2; j <= 14; j += 2) {// идем по каждой второй колонке
                WallsInsideTheMapOne(i, j); // вызываем метод вывода стены с номером два
            }
        }
    }

    void WallsInsideTheMapTwo() {
        int** bomber = maze->GetBomber();
        for (int y = 0; y < maze->GetHeight(); y++)
        {
            for (int x = 0; x < maze->GetWidth(); x++) {
                int r = rand() % 10; // 0...9
                if (r == 1) {// если выпадает не 0 (а число от 1 до 9) - врага убираем (вероятность 90%)
                    bomber[y][x] = 1; // убираем врага
                }
                if (bomber[y][x] == 1) {//стена
                    SetCursor(x, y, 3);
                    cout << (char)178;
                }
            }
        }
    }

    void WallsInsideTheMapThree() {
        int** bomber = maze->GetBomber();
        for (int y = 0; y < maze->GetHeight(); y++) {
            for (int x = 0; x < maze->GetWidth(); x++) {
                int r = rand() % 10; // 0...9
                if (r == 1 && bomber[y][x] != WALL) { // если выпадает не 0 (а число от 1 до 9) - врага убираем (вероятность 90%)
                    bomber[y][x] = WALLTHREE; // убираем врага
                }
                if (bomber[y][x] == WALLTHREE) {//стена
                    SetCursor(x, y, 7);
                    cout << (char)178;
                }
            }
        }
    }

    ~Wall() {
        if (maze != nullptr) {
            //delete maze;
        }
    }
};

class Game {
public:

    void PlayGame() {
        PlayS("C:\\Users\\lolim\\Documents\\MusicBomberMan\\main.wav", 60);
        srand(time(NULL));
        system("title Bomberman");
        Maze maze(61, 17);
        Bomb bomb;
        Wall w(&maze);
        Enemy enemy(&maze);
        Bomber b(&maze, &bomb, &enemy);
        enemy.EnemyGeneration();
        w.WallGenerate();
        w.AutomateWallNumberTwo();
        w.WallsInsideTheMapTwo();
        w.WallsInsideTheMapThree();
        b.Person(2, 2);
    }
};

class Menu {
    const int NUM_MENU_ITEMS = 3;
    int ActiveMenuItem = 0; // Выбранный пункт меню
    int ch = 0; // Хранение нажатой клавиши
    bool exit = false; // для выхода из цикла
    Game game;
public:
    void gotoxy(int x, int y) {
        COORD coord;
        coord.X = x;
        coord.Y = y;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    }

    void Exit()
    {
        HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(h, 0);
        system("taskkill -im cmd.exe"); // закрываем все процессы по имени cmd - это все консольные окна
    }

    void Menu2()
    {
        HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
        system("title Maze");
        int start_y = 3;
        system("cls");
        Stand(5, start_y, 10, "Help");
        Stand(5, start_y + 2, 2, "Game");
        Stand(5, start_y + 4, 2, "Exit");

        int k;
        int cur = 1;
        while (true)
        {
            k = _getch();
            if (k == 80)
            {
                PlayS("C:\\Users\\lolim\\Documents\\MusicBomberMan\\clixk.wav", 30);
                if (cur < 3)
                {
                    cur++;
                    if (cur == 2)
                    {
                        Stand(5, start_y, 2, "Help");
                        Stand(5, start_y + 2, 10, "Game");
                    }
                    else if (cur == 3) {
                        Stand(5, start_y + 2, 2, "Game");
                        Stand(5, start_y + 4, 10, "Exit");
                    }
                }
            }
            else if (k == 72)
            {
                PlayS("C:\\Users\\lolim\\Documents\\MusicBomberMan\\clixk.wav", 30);
                if (cur > 1)
                {
                    cur--;
                    if (cur == 2)
                    {
                        Stand(5, start_y + 2, 10, "Game");
                        Stand(5, start_y + 4, 2, "Exit");
                    }
                    else if (cur == 1)
                    {
                        Stand(5, start_y, 10, "Help");
                        Stand(5, start_y + 2, 2, "Game");
                    }
                }
            }
            else if (k == 13)
            {
                PlayS("C:\\Users\\lolim\\Documents\\MusicBomberMan\\clixk.wav", 30);
                system("cls");
                SetConsoleTextAttribute(h, 12);
                if (cur == 1)
                {
                    Help();
                }
                else if (cur == 2)
                {
                    PlayS("C:\\Users\\lolim\\Documents\\MusicBomberMan\\clixk.wav", 30);
                    game.PlayGame();
                }
                else if (cur == 3)
                {
                    PlayS("C:\\Users\\lolim\\Documents\\MusicBomberMan\\clixk.wav", 30);
                    The_end();

                    ::exit(0);
                }
                
            }
        }
    }

    void The_end()
    {
        Text_effect(5, 2, 12, 0, "Thank you for playing!", 30, 20); // красным цветом благодарим за игру
        Text_effect(5, 4, 7, 0, "Press Enter to exit", 24, 15); // белым цветом выводим просьбу нажать на энтер
        while (_getch() != 13); // до тех пор, пока пользователь не нажмёт на энтер - у него код 13
        Exit(); // выходим
    }

    // Вывод лого
    void ShowLogo() {
        gotoxy(50, 15);
        cout << "BOMBERMAN!!!" << "\n";
        Sleep(1000);
    }
};

void Help()
{

    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(h, 10);
    system("type aboutgame.txt"); // стандартная команда виндовс по распечатке файла (файл лежит в папке с проектом!)

    while (true)
    {
        int n = _getch();
        if (n == 27 || n == 'n')
        {
            Menu m;
            m.Menu2();
        }
    }
}

int main()
{
    Options();
    Menu menu;
    menu.ShowLogo();
    menu.Menu2();
}
