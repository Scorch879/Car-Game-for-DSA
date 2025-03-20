#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <stdbool.h>
#include <windows.h>
#include <time.h>

// Game constants
#define WIDTH 50
#define HEIGHT 25
#define CAR_WIDTH 1
#define OBSTACLE_WIDTH 3
#define BOSS_WIDTH 9
#define LANE_COUNT 3
#define LANE_WIDTH (WIDTH / (LANE_COUNT + 1))
#define MAX_OBSTACLES 10
#define INITIAL_LIVES 3
#define BASE_SPEED 80
#define MIN_SPEED 20
#define SLOWMO_DURATION 50  // ticks

int i = 0;
int score = 0;
int carLane = 1;
bool gameOver = false;
int lives = INITIAL_LIVES;
int level = 1;
int speed = BASE_SPEED;
bool shield = false;
bool slowmo = false;
int slowmoTicks = 0;
int highScore = 0;

void gameLoop();
void restartGame();
HANDLE hConsole; // Console handle for colors

typedef struct {
    int lane;
    float y;
    bool active;
    bool boss;
} Obstacle;

Obstacle obstacles[MAX_OBSTACLES];

void setColor(int color) {
    SetConsoleTextAttribute(hConsole, color);
}

void gotoxy(int x, int y) {
    COORD coord = {x, y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void hideCursor() {
    CONSOLE_CURSOR_INFO cursorInfo;
    cursorInfo.dwSize = 1;
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

void drawBorder() {
    int i;
    setColor(7);
    for (i = 0; i <= HEIGHT; i++) {
        gotoxy(0, i); printf("|");
        gotoxy(WIDTH, i); printf("|");
    }
}

int getLaneX(int lane) {
    int laneCenter = (lane + 1) * LANE_WIDTH;
    return laneCenter - (CAR_WIDTH / 2);
}

void drawCar() {
    int x = getLaneX(carLane);
    int i;
    setColor(shield ? 11 : 10);
    for (i = 0; i < CAR_WIDTH; i++) {
        gotoxy(x + i, HEIGHT - 2);
        printf("A");
    }
    setColor(7);
}

void clearCar() {
    int lane, i;
    for (lane = 0; lane < LANE_COUNT; lane++) {
        int x = getLaneX(lane);
        for (i = 0; i < CAR_WIDTH; i++) {
            gotoxy(x + i, HEIGHT - 2);
            printf(" ");
        }
    }
}

void clearObstacles() {
    int i, j;
    for (i = 0; i < MAX_OBSTACLES; i++) {
        if (obstacles[i].active) {
            int x = getLaneX(obstacles[i].lane);
            int yPos = (int)(obstacles[i].y);
            int width = obstacles[i].boss ? BOSS_WIDTH : OBSTACLE_WIDTH;
            for (j = 0; j < width; j++) {
                gotoxy(x + j, yPos);
                printf(" ");
            }
        }
    }
}

void drawObstacles() {
    int i, j;
    for (i = 0; i < MAX_OBSTACLES; i++) {
        if (obstacles[i].active) {
            int x = getLaneX(obstacles[i].lane);
            int yPos = (int)(obstacles[i].y);
            int width = obstacles[i].boss ? BOSS_WIDTH : OBSTACLE_WIDTH;

            setColor(obstacles[i].boss ? 12 : 14);
            for (j = 0; j < width; j++) {
                gotoxy(x + j, yPos);
                printf("%c", obstacles[i].boss ? '#' : 'X');
            }
        }
    }
    setColor(7);
}

void explosionEffect(int x, int y) {
    int i;
    setColor(12);
    for (i = 0; i < 3; i++) {
        gotoxy(x, y);     printf("* * *");
        gotoxy(x, y + 1); printf(" * * ");
        gotoxy(x, y + 2); printf("* * *");
        Sleep(100);
        gotoxy(x, y);     printf("       ");
        gotoxy(x, y + 1); printf("       ");
        gotoxy(x, y + 2); printf("       ");
        Sleep(100);
    }
    setColor(7);
}

void updateObstacles() {
    int i;
    clearObstacles();

    for (i = 0; i < MAX_OBSTACLES; i++) {
        if (obstacles[i].active) {
            obstacles[i].y += slowmo ? 0.2 : 0.5;

            if (obstacles[i].y >= HEIGHT - 1) {
                obstacles[i].active = false;
                score++;

                level = score / 10 + 1;
                speed = BASE_SPEED - (level - 1) * 5;
                if (speed < MIN_SPEED) speed = MIN_SPEED;
            }

            int carX = getLaneX(carLane);
            int obsX = getLaneX(obstacles[i].lane);
            int obsWidth = obstacles[i].boss ? BOSS_WIDTH : OBSTACLE_WIDTH;

            bool xOverlap = (carX < obsX + obsWidth) && (carX + CAR_WIDTH > obsX);
            bool yOverlap = ((int)(obstacles[i].y) == HEIGHT - 2);

            if (xOverlap && yOverlap) {
                obstacles[i].active = false;

                if (shield) {
                    shield = false;
                } else {
                    explosionEffect(carX, HEIGHT - 3);
                    lives--;
                    if (lives <= 0) {
                        gameOver = true;
                    }
                }
            }
        }
    }
}

void spawnObstacle() {
    int i;
    for (i = 0; i < MAX_OBSTACLES; i++) {
        if (!obstacles[i].active) {
            obstacles[i].active = true;
            obstacles[i].y = 0;
            obstacles[i].lane = rand() % LANE_COUNT;
            obstacles[i].boss = (level % 5 == 0 && rand() % 3 == 0);
            break;
        }
    }
}

void drawUI() {
    gotoxy(WIDTH + 5, 2); setColor(15); printf("Score : %d", score);
    gotoxy(WIDTH + 5, 3); printf("Level : %d", level);
    gotoxy(WIDTH + 5, 4); printf("Lives : %d", lives);
    gotoxy(WIDTH + 5, 5); printf("High  : %d", highScore);
    gotoxy(WIDTH + 5, 7); setColor(shield ? 11 : 8); printf("Shield : %s", shield ? "ON " : "OFF");
    gotoxy(WIDTH + 5, 8); setColor(slowmo ? 13 : 8); printf("SlowMo : %s", slowmo ? "ON " : "OFF");
    setColor(7);
    gotoxy(WIDTH + 5, 10); printf("Controls: A/D");
    gotoxy(WIDTH + 5, 11); printf("PowerUp : S");
    gotoxy(WIDTH + 5, 12); printf("Exit    : X");
}

void levelUpEffect() {
    int i;
    setColor(10);
    for (i = 0; i < 3; i++) {
        gotoxy(WIDTH / 2 - 5, HEIGHT / 2);
        printf("LEVEL %d!", level);
        Sleep(200);
        gotoxy(WIDTH / 2 - 5, HEIGHT / 2);
        printf("         ");
        Sleep(200);
    }
    setColor(7);
}

void saveHighScore() {
    if (score > highScore) {
        FILE *file = fopen("highscore.txt", "w");
        if (file != NULL) {
            fprintf(file, "%d", score);
            fclose(file);
        }
    }
}

void loadHighScore() {
    FILE *file = fopen("highscore.txt", "r");
    if (file != NULL) {
        fscanf(file, "%d", &highScore);
        fclose(file);
    }
}

void activatePowerUp() {
    int power = rand() % 2;
    if (power == 0) {
        shield = true;
    } else {
        slowmo = true;
        slowmoTicks = SLOWMO_DURATION;
    }
}

void gameOverScreen() {
    system("cls");
    setColor(12);
    gotoxy(WIDTH / 2 - 5, HEIGHT / 2 - 2);
    printf(" GAME OVER! ");
    gotoxy(WIDTH / 2 - 7, HEIGHT / 2);
    printf("Score: %d", score);
    gotoxy(WIDTH / 2 - 10, HEIGHT / 2 + 2);
    printf("Press R to Restart or X to Exit");

    char input;
    while (1) {
        if (_kbhit()) {
            input = _getch();
            if (input == 'r' || input == 'R') {
                restartGame();
                break;
            } else if (input == 'x' || input == 'X') {
                exit(0);
            }
        }
        Sleep(100);
    }
}

void restartGame() {
    score = 0;
    carLane = 1;
    gameOver = false;
    lives = INITIAL_LIVES;
    level = 1;
    speed = BASE_SPEED;
    shield = false;
    slowmo = false;
    slowmoTicks = 0;

    for ( i = 0; i < MAX_OBSTACLES; i++) {
        obstacles[i].active = false;
    }

    system("cls");
    gameLoop();
}

void gameLoop() {
    int tick = 0;
    int lastLevel = level;

    hideCursor();
    drawBorder();
    loadHighScore();

    while (!gameOver) {
        clearCar();

        if (_kbhit()) {
            char ch = _getch();
            if (ch == 'a' || ch == 'A') {
                if (carLane > 0) carLane--;
            } else if (ch == 'd' || ch == 'D') {
                if (carLane < LANE_COUNT - 1) carLane++;
            } else if (ch == 's' || ch == 'S') {
                activatePowerUp();
            } else if (ch == 'x' || ch == 'X') {
                gameOver = true;
            }
        }

        if (tick % 2 == 0) {
            updateObstacles();

            int spawnChance = 5 - level;
            if (spawnChance < 1) spawnChance = 1;

            if (rand() % spawnChance == 0) {
                spawnObstacle();
            }
        }

        if (level > lastLevel) {
            levelUpEffect();
            lastLevel = level;
        }

        if (slowmo) {
            slowmoTicks--;
            if (slowmoTicks <= 0) {
                slowmo = false;
            }
        }

        drawCar();
        drawObstacles();
        drawUI();

        Sleep(speed);
        tick++;
    }

    saveHighScore();
    gameOverScreen();
}

int main() {
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    srand(time(NULL));
    system("cls");

    gameLoop();

    return 0;
}
