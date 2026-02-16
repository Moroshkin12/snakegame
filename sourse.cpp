#include <iostream>
#include <Windows.h>
#include <ctime>

void gotoxy(int x, int y) {
    COORD pos = { x, y };
    HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(output, pos); // функция решает проблему "кинопленки" возращает курсор в началу и отрисовывает карту
}

const int WIDTH = 17;
const int HEIGHT = 10;
const int MAX_LEN_SNAKE = (WIDTH - 3) * (HEIGHT - 2);

const int UP = 0;
const int DOWN = 1;
const int LEFT = 2;
const int RIGHT = 3;   // константы отвечающие за направление движения змейки

int snake_dir = UP;
bool isRunning = true;

char map[] =
"################\n"
"#              #\n"
"#              #\n"
"#              #\n"
"#              #\n"
"#              #\n"
"#              #\n"
"#              #\n"
"#              #\n"
"################\n";

char snake = 'O';
char food = '*';

int snake_x[MAX_LEN_SNAKE] = { 0 };
int snake_y[MAX_LEN_SNAKE] = { 0 }; // координаты змейки
int snake_len = 1;

int food_x = 1 + (rand() % (WIDTH - 3));
int food_y = 1 + (rand() % (HEIGHT - 2)); // определяем позицию еды на поле рандомно

int main() {
    srand(time(NULL));
    setlocale(LC_ALL, "RU");

    snake_x[0] = WIDTH / 2;
    snake_y[0] = HEIGHT / 2; // начальное положение змейки
    double time = clock();

    
    bool food_eaten = false;

    while (isRunning) {
        if (GetKeyState('W') & 0x8000) {
            if (snake_dir != DOWN) {
                snake_dir = UP;
            }
        }
        if (GetKeyState('A') & 0x8000) {
            if (snake_dir != RIGHT) {
                snake_dir = LEFT;
            }
        }
        if (GetKeyState('S') & 0x8000) {
            if (snake_dir != UP) {
                snake_dir = DOWN;
            }
        }
        if (GetKeyState('D') & 0x8000) {
            if (snake_dir != LEFT) {
                snake_dir = RIGHT;
            }
        }
        

        
        if ((clock() - time) / CLOCKS_PER_SEC >= 0.3) {
            time = clock();

            // Запоминаем позицию хвоста
            int tail_x = snake_x[snake_len - 1];
            int tail_y = snake_y[snake_len - 1];

            // Сдвигаем тело змейки (кроме головы)
            for (int i = snake_len - 1; i > 0; i--) {
                snake_x[i] = snake_x[i - 1];
                snake_y[i] = snake_y[i - 1];
            }

            // Двигаем голову
            if (snake_dir == UP) {
                snake_y[0]--;
            }
            if (snake_dir == DOWN) {
                snake_y[0]++;
            }
            if (snake_dir == RIGHT) {
                snake_x[0]++;
            }
            if (snake_dir == LEFT) {
                snake_x[0]--;
            }

            // Проверка на еду
            if (snake_x[0] == food_x && snake_y[0] == food_y) {
                food_eaten = true; 

                
                food_x = 1 + (rand() % (WIDTH - 3));
                food_y = 1 + (rand() % (HEIGHT - 2));
            }

            
            if (food_eaten) {
                snake_len++;
                // Добавляем новую часть тела на месте бывшего хвоста
                snake_x[snake_len - 1] = tail_x;
                snake_y[snake_len - 1] = tail_y;
                food_eaten = false; // Сбрасываем флаг
            }

           
            if (snake_x[0] <= 0 || snake_y[0] <= 0 ||
                snake_x[0] >= WIDTH - 2 || snake_y[0] >= HEIGHT - 1) {
                isRunning = false;
            }

            
            for (int i = 1; i < snake_len; i++) {
                if (snake_x[0] == snake_x[i] && snake_y[0] == snake_y[i]) {
                    isRunning = false;
                    break;
                }
            }

            // Отрисовка
            gotoxy(0, 0);
            std::cout << "Длина змейки: " << snake_len << "\n";

            // Очищаем карту
            for (int i = 0; i < HEIGHT * WIDTH; i++) {
                if (map[i] == snake) {
                    map[i] = ' ';
                }
            }

            // Размещаем еду
            map[food_y * WIDTH + food_x] = food;

            // Размещаем змейку
            for (int i = 0; i < snake_len; i++) {
                map[snake_y[i] * WIDTH + snake_x[i]] = snake;
            }

            std::cout << map;
        }
    }

    
    gotoxy(1, HEIGHT / 2);
    std::cout << "Ваши очки: " << snake_len;
    gotoxy(WIDTH, HEIGHT);

    return 0;
}
