#include <iostream>
#include <ctime>
#include <Windows.h>
#include <conio.h> 
using namespace std;

enum MazeObject { HALL = 0, WALL = 1, WALLTWO = 2, WALLTHREE = 3, ENEMY = 4, BOMB = 5, HEALTH = 6 };
enum Color { DARKGREEN = 2, YELLOW = 14, RED = 12, BLUE = 9, WHITE = 15, DARKYELLOW = 6, DARKRED = 4, PURPUR = 13, GREEN = 10 };
enum KeyCode { ENTER = 13, ESCAPE = 27, SPACE = 32, LEFT = 75, RIGHT = 77, UP = 72, DOWN = 80 };

class Maze {
private:
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    int width;
    int height;
    int** bomber = nullptr;
public:

    Maze() {
        SetWidth(61);
        SetHeight(17);
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

    void Generation() {//сделал генерацию для массива bomber
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                bomber[y][x] = rand() % 7;
            }
        }
    }

    Maze(int w, int h) : width(w), height(h) {
        bomber = new int* [height];
        for (int i = 0; i < height; i++) {
            bomber[i] = new int[width];
        }
        Generation();//метод находится в КСП для того чтоб генерация была доступна для всех мктодов класса
    }

    void SetCursor(int x, int y, int color) {
        COORD position;
        position.X = x;
        position.Y = y;
        //cout << position.Y << "\n";
        //cout << position.X << "\n";
        SetConsoleCursorPosition(h, position);
        SetConsoleTextAttribute(h, color);
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

    void KeyBoard(int x, int y) {
        COORD position;
        position.X = x;
        position.Y = y;
    }

    void Options() {
        HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
        // размер окна - 130х50 символов
        CONSOLE_CURSOR_INFO ci; // скрытие курсора
        ci.bVisible = false; // курсор не видно
        ci.dwSize = 100; // на самом деле, не важно что сюда писать - подходит любое значение от 1 до 100 (рамер курсора в процентах относительно клетки)
        SetConsoleCursorInfo(h, &ci); // применить настройки
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

    Enemy(Maze* m) : maze(m) {
        if (m != nullptr) {
            maze = new Maze(*m); // Создание нового объекта Maze путем копирования
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
                    maze->SetCursor(x, y, RED);
                    cout << (char)224;
                }
            }
        }
    }

    void Joystick() {
        int** bomber = maze->GetBomber();
        Sleep(15);
        // движение врагов
        COORD enemy_positions[100]; // массив который хранит координаты врагов
        int enemy_count = 0; // реальное количество врагов на данный момент после шага ГГ
        // перебор всех ячеек которые есть в лабиринте
        for (int y = 0; y < maze->GetHeight(); y++) {// перебор строк
            for (int x = 0; x < maze->GetWidth(); x++) { // перебор столбцов
                // если очередная ячейка - это враг
                if (bomber[y][x] == MazeObject::ENEMY) {
                    enemy_positions[enemy_count].X = x;
                    enemy_positions[enemy_count].Y = y;
                    enemy_count++;
                }
            }
        }
        // перебор всех врагов
        for (int i = 0; i < enemy_count; i++) {
            int r = rand() % 100; // 0-left, 1-right, 2-up, 3-down
            if (r == LEFT &&
                bomber[enemy_positions[i].Y][enemy_positions[i].X - 1] != MazeObject::WALL &&
                bomber[enemy_positions[i].Y][enemy_positions[i].X - 1] != MazeObject::WALLTWO &&
                bomber[enemy_positions[i].Y][enemy_positions[i].X - 1] != MazeObject::ENEMY &&
                bomber[enemy_positions[i].Y][enemy_positions[i].X - 1] != MazeObject::BOMB &&
                bomber[enemy_positions[i].Y][enemy_positions[i].X - 1] != MazeObject::HEALTH &&
                bomber[enemy_positions[i].Y][enemy_positions[i].X - 1] != MazeObject::WALLTHREE) {
                // стирание врага в старой позиции
                COORD temp = enemy_positions[i];
                SetConsoleCursorPosition(maze->GetH(), temp);
                cout << " ";
                bomber[enemy_positions[i].Y][enemy_positions[i].X] = MazeObject::HALL;
                // перемещаем врага в новую позицию
                temp.X = enemy_positions[i].X - 1;
                temp.Y = enemy_positions[i].Y;
                SetConsoleCursorPosition(maze->GetH(), temp);
                SetConsoleTextAttribute(maze->GetH(), Color::RED);
                cout << (char)224;
                bomber[enemy_positions[i].Y][enemy_positions[i].X - 1] = MazeObject::ENEMY;
            }

            else if (r == RIGHT &&
                bomber[enemy_positions[i].Y][enemy_positions[i].X + 1] != MazeObject::WALL &&
                bomber[enemy_positions[i].Y][enemy_positions[i].X + 1] != MazeObject::WALLTWO &&
                bomber[enemy_positions[i].Y][enemy_positions[i].X + 1] != MazeObject::ENEMY &&
                bomber[enemy_positions[i].Y][enemy_positions[i].X + 1] != MazeObject::BOMB &&
                bomber[enemy_positions[i].Y][enemy_positions[i].X + 1] != MazeObject::HEALTH &&
                bomber[enemy_positions[i].Y][enemy_positions[i].X + 1] != MazeObject::WALLTHREE) {
                // стирание врага в старой позиции
                COORD temp = enemy_positions[i];
                SetConsoleCursorPosition(maze->GetH(), temp);
                cout << " ";
                bomber[enemy_positions[i].Y][enemy_positions[i].X] = MazeObject::HALL;
                // перемещаем врага в новую позицию
                temp.X = enemy_positions[i].X + 1;
                temp.Y = enemy_positions[i].Y;
                SetConsoleCursorPosition(maze->GetH(), temp);
                SetConsoleTextAttribute(maze->GetH(), Color::RED);
                cout << (char)224;
                bomber[enemy_positions[i].Y][enemy_positions[i].X + 1] = MazeObject::ENEMY;
            }

            else if (r == UP &&
                bomber[enemy_positions[i].Y - 1][enemy_positions[i].X] != MazeObject::WALL &&
                bomber[enemy_positions[i].Y - 1][enemy_positions[i].X] != MazeObject::WALLTWO &&
                bomber[enemy_positions[i].Y - 1][enemy_positions[i].X] != MazeObject::ENEMY &&
                bomber[enemy_positions[i].Y - 1][enemy_positions[i].X] != MazeObject::BOMB &&
                bomber[enemy_positions[i].Y - 1][enemy_positions[i].X] != MazeObject::HEALTH &&
                bomber[enemy_positions[i].Y - 1][enemy_positions[i].X] != MazeObject::WALLTHREE) {
                // стирание врага в старой позиции
                COORD temp = enemy_positions[i];
                SetConsoleCursorPosition(maze->GetH(), temp);
                cout << " ";
                bomber[enemy_positions[i].Y][enemy_positions[i].X] = MazeObject::HALL;
                // перемещаем врага в новую позицию
                temp.X = enemy_positions[i].X;
                temp.Y = enemy_positions[i].Y - 1;
                SetConsoleCursorPosition(maze->GetH(), temp);
                SetConsoleTextAttribute(maze->GetH(), Color::RED);
                cout << (char)224;
                bomber[enemy_positions[i].Y - 1][enemy_positions[i].X] = MazeObject::ENEMY;
            }

            else if (r == DOWN &&
                bomber[enemy_positions[i].Y + 1][enemy_positions[i].X] != MazeObject::WALL &&
                bomber[enemy_positions[i].Y + 1][enemy_positions[i].X] != MazeObject::WALLTWO &&
                bomber[enemy_positions[i].Y + 1][enemy_positions[i].X] != MazeObject::ENEMY &&
                bomber[enemy_positions[i].Y + 1][enemy_positions[i].X] != MazeObject::BOMB &&
                bomber[enemy_positions[i].Y + 1][enemy_positions[i].X] != MazeObject::HEALTH &&
                bomber[enemy_positions[i].Y + 1][enemy_positions[i].X] != MazeObject::WALLTHREE) {
                // стирание врага в старой позиции
                COORD temp = enemy_positions[i];
                SetConsoleCursorPosition(maze->GetH(), temp);
                cout << " ";
                bomber[enemy_positions[i].Y][enemy_positions[i].X] = MazeObject::HALL;
                // перемещаем врага в новую позицию
                temp.X = enemy_positions[i].X;
                temp.Y = enemy_positions[i].Y + 1;
                SetConsoleCursorPosition(maze->GetH(), temp);
                SetConsoleTextAttribute(maze->GetH(), Color::RED);
                cout << (char)224;
                bomber[enemy_positions[i].Y + 1][enemy_positions[i].X] = MazeObject::ENEMY;
            }
        }
    }
};

class Bomb {
private:
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
        
        maze->SetCursor(x, y, 12);
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
                    position.X++; // изменение позиции ГГ вправо на 1 по иксу
                }
                else if (code == LEFT && maze->GetBomber()[position.Y][position.X - 1] != WALL && maze->GetBomber()[position.Y][position.X - 1] != WALLTWO && maze->GetBomber()[position.Y][position.X - 1] != WALLTHREE) {
                    position.X--;// изменение позиции ГГ влево на 1 по иксу            
                }
                else if (code == UP && maze->GetBomber()[position.Y - 1][position.X] != WALL && maze->GetBomber()[position.Y - 1][position.X] != WALLTWO && maze->GetBomber()[position.Y - 1][position.X] != WALLTHREE) {
                    position.Y--;// изменение позиции ГГ вверх на 1 по иксу          
                }
                else if (code == DOWN && maze->GetBomber()[position.Y + 1][position.X] != WALL && maze->GetBomber()[position.Y + 1][position.X] != WALLTWO && maze->GetBomber()[position.Y + 1][position.X] != WALLTHREE) {
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
                        maze->SetCursor(bomb->GetbX(), bomb->GetbY(), PURPUR);
                        cout << (char)254;
                    }
                }

                else if (code == 103 && bomb->GetCountOfBomb() != 0) {
                    if (bomb->GetBomb() == true) {
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
                                                maze->SetCursor(newX, newY, PURPUR);
                                                cout << (char)254;
                                            }
                                            if (a == 6) {
                                                maze->GetBomber()[newY][newX] = HEALTH;
                                                maze->SetCursor(newX, newY, 14);
                                                cout << (char)3;
                                            }
                                        }
                                        else {
                                            maze->GetBomber()[newY][newX] = 0;
                                            maze->SetCursor(newX, newY, 0);
                                            cout << ' ';
                                        }
                                    }
                                }
                            }
                        }
                    }
                }

                maze->SetCursor(position.X, position.Y, 12);
                cout << (char)1;

                if (position.X == maze->GetWidth() - 2 and position.Y == maze->GetHeight() - 2) {
                    MessageBoxA(0, "You found your way out!!!!", "You win!!!", MB_OK);
                    break;
                }

                maze->SetCursor(maze->GetWidth() + 1, 1, PURPUR);
                cout << "Hit points: ";
                cout << health_person;

                if (maze->GetBomber()[position.Y][position.X] == ENEMY) {
                    health_person -= 20;
                    maze->GetBomber()[position.Y][position.X] = 0;
                    maze->SetCursor(maze->GetWidth() + 1, 1, PURPUR);
                    cout << "Hit points: ";
                    cout << health_person;
                    cout << " ";
                }
                if (maze->GetBomber()[position.Y][position.X] == HEALTH and health_person < 100) {
                    health_person += 20;
                    maze->GetBomber()[position.Y][position.X] = 0;
                    maze->SetCursor(maze->GetWidth() + 1, 1, PURPUR);
                    cout << "Hit points: ";
                    cout << health_person;
                    cout << " ";
                }

                maze->SetCursor(maze->GetWidth() + 1, 3, YELLOW);
                cout << "Number of bombs: ";
                cout << bomb->GetCountOfBomb();
                cout << " ";

                if (code == 103) {
                    maze->SetCursor(maze->GetWidth() + 1, 3, YELLOW);
                    cout << "Number of bombs: ";
                    cout << bomb->GetCountOfBomb();
                    cout << " ";
                }

                if (maze->GetBomber()[position.Y][position.X] == BOMB and bomb->GetCountOfBomb() < 10) {
                    bomb->SetPlusBomb(1);
                    
                    maze->GetBomber()[position.Y][position.X] = 0;
                    maze->SetCursor(maze->GetWidth() + 1, 3, YELLOW);
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
        if (maze != nullptr) {
            delete maze;
        }
        if (enemy != nullptr) {
            delete enemy;
        }
        if (bomb != nullptr) {
            delete bomb;
        }
    }
};

class Wall {
private:
    Maze* maze = nullptr;
public:

    Wall(Maze* m) {
        if (m != nullptr) {
            maze = new Maze(*m); // Создание нового объекта Maze путем копирования
        }
    }

    Wall(const Wall& original) {
        this->maze = new Maze(*original.maze);
    }

    void WallGenerate() {
        int** bomber = maze->GetBomber();
        for (int y = 0; y < maze->GetHeight(); y++) {
            for (int x = 0; x < maze->GetWidth(); x++) {
                if (x == 0 || y == 0 || x == maze->GetWidth() - 1 || y == maze->GetHeight() - 1) {
                    maze->SetCursor(x, y, 8);
                    cout << (char)178;
                }
                if (bomber[y][x] == 0) {//коридор
                    maze->SetCursor(x, y, 0);
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
                    maze->SetCursor(x, y, 3);
                    cout << (char)178;
                }
            }
        }
    }

    void WallsInsideTheMapThree() {
        int** bomber = maze->GetBomber();
        for (int y = 0; y < maze->GetHeight(); y++) {
            for (int x = 0; x < maze->GetHeight(); x++) {
                int r = rand() % 10; // 0...9
                if (r == 1 && bomber[y][x] != WALL) { // если выпадает не 0 (а число от 1 до 9) - врага убираем (вероятность 90%)
                    bomber[y][x] = WALLTHREE; // убираем врага
                }
                if (bomber[y][x] == WALLTHREE) {//стена
                    maze->SetCursor(x, y, 7);
                    cout << (char)178;
                }
            }
        }
    }

    ~Wall() {
        if (maze != nullptr) {
            delete maze;
        }
    }
};

int main()
{
    srand(time(NULL));
    system("title Bomberman");
    Maze maze(61, 17);
    Bomb bomb;
    Wall w(&maze);
    Enemy enemy(&maze);

    Bomber b(&maze, &bomb, &enemy);
    maze.Options();
    enemy.EnemyGeneration();
    w.WallGenerate();
    w.AutomateWallNumberTwo();
    w.WallsInsideTheMapTwo();
    w.WallsInsideTheMapThree();
    b.Person(2, 2);
}
