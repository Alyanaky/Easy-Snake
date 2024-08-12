#include <iostream>
#include <windows.h>
#include <conio.h>
#include <ctime>
#include <vector>

using namespace std;

class Snake {
private:
    int width;
    int height;
    int maxLength;
    int length;
    vector<int> xCoords;
    vector<int> yCoords;
    int dx;
    int dy;
    int appleX;
    int appleY;
    int sleepTime;
    HANDLE hConsole;
    COORD c;
    bool gameOver;

    void generateApple() {
        do {
            appleX = rand() % (width - 2) + 1;
            appleY = rand() % (height - 2) + 1;
        } while (checkCollision(appleX, appleY));
    }

    bool checkCollision(int x, int y) {
        for (int i = 0; i < length; ++i) {
            if (x == xCoords[i] && y == yCoords[i]) {
                return true;
            }
        }
        return false;
    }

    void drawFrame() {
        SetConsoleTextAttribute(hConsole, 4);
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                if (x == 0 && y == 0) {
                    cout << '#';
                }
                else if (x == 0 && y == height - 1) {
                    cout << '#';
                }
                else if (y == 0 && x == width - 1) {
                    cout << '#';
                }
                else if (y == height - 1 && x == width - 1) {
                    cout << '#';
                }
                else if (y == 0 || y == height - 1) {
                    cout << '#';
                }
                else if (x == 0 || x == width - 1) {
                    cout << '#';
                }
                else {
                    cout << ' ';
                }
            }
            cout << endl;
        }
    }

    void drawSnake() {
        for (int i = 0; i < length; ++i) {
            c.X = xCoords[i];
            c.Y = yCoords[i];
            SetConsoleCursorPosition(hConsole, c);
            if (i == length - 1) {
                SetConsoleTextAttribute(hConsole, 10); // Зеленый цвет для головы
                cout << 1; // Символ головы
            }
            else {
                SetConsoleTextAttribute(hConsole, 10); // Зеленый цвет для тела
                cout << '*'; // Символ тела
            }
        }
    }

    void drawApple() {
        c.X = appleX;
        c.Y = appleY;
        SetConsoleCursorPosition(hConsole, c);
        SetConsoleTextAttribute(hConsole, 12); // Красный цвет для яблока
        cout << 'o'; // Символ яблока
    }

    void move() {
        int newX = xCoords[length - 1] + dx;
        int newY = yCoords[length - 1] + dy;
        if (newX == 0 || newX == width - 1 || newY == 0 || newY == height - 1) {
            gameOver = true;
            return;
        }

        if (checkCollision(newX, newY)) {
            gameOver = true;
            return;
        }

        if (newX == appleX && newY == appleY) {
            // Поедание яблока
            c.X = xCoords[length - 1];
            c.Y = yCoords[length - 1];
            SetConsoleCursorPosition(hConsole, c);
            cout << '*'; // Отрисовка тела на месте головы

            length++;
            xCoords.push_back(newX);
            yCoords.push_back(newY);
            if (length == maxLength) {
                gameOver = true; // Победа
            }
            generateApple();
            drawApple();
        }
        else {
            // Движение без поедания яблока
            c.X = xCoords[0];
            c.Y = yCoords[0];
            SetConsoleCursorPosition(hConsole, c);
            cout << ' '; // Стирание хвоста

            for (int i = 0; i < length - 1; ++i) {
                xCoords[i] = xCoords[i + 1];
                yCoords[i] = yCoords[i + 1];
            }

            xCoords[length - 1] = newX;
            yCoords[length - 1] = newY;
        }

        drawSnake();
    }

    void handleInput() {
        if (_kbhit()) {
            int key = _getch();
            if (key == 0 || key == 224) {
                key = _getch();
            }
            switch (key) {
            case 80: // Вниз
                if (dy != -1) { // Запрет движения вверх, если змея уже движется вниз
                    dy = 1;
                    dx = 0;
                }
                break;
            case 72: // Вверх
                if (dy != 1) { // Запрет движения вниз, если змея уже движется вверх
                    dy = -1;
                    dx = 0;
                }
                break;
            case 75: // Влево
                if (dx != 1) { // Запрет движения вправо, если змея уже движется влево
                    dy = 0;
                    dx = -1;
                }
                break;
            case 77: // Вправо
                if (dx != -1) { // Запрет движения влево, если змея уже движется вправо
                    dy = 0;
                    dx = 1;
                }
                break;
            case 27: // ESC
                gameOver = true;
                break;
            }
        }
    }

public:
    Snake(int w = 50, int h = 30, int ml = 50, int st = 100) :
        width(w), height(h), maxLength(ml), length(1), dx(1), dy(0),
        sleepTime(st), gameOver(false) {
        hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        xCoords.push_back(width / 2);
        yCoords.push_back(height / 2);
        generateApple();

        CONSOLE_CURSOR_INFO cci = { sizeof(cci), false };
        SetConsoleCursorInfo(hConsole, &cci);
    }

    void start() {
        system("title Snake Game");
        system("mode con cols=70 lines=31");
        MoveWindow(GetConsoleWindow(), 50, 50, 2000, 2000, true);

        drawFrame();
        drawSnake();
        drawApple();

        while (!gameOver) {
            Sleep(sleepTime);
            handleInput();
            move();
        }

        system("cls");
        if (length == maxLength) {
            cout << "You Win!" << endl;
        }
        else {
            cout << "Game Over!" << endl;
        }
    }
};

int main() {
    srand(time(0));
    Snake game;
    game.start();
    return 0;
}
