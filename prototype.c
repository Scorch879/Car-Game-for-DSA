#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <stdbool.h>
#include <windows.h>
#include <time.h>

// Game constants
#define WIDTH 50
#define HEIGHT 25
#define CAR_WIDTH 5
#define OBSTACLE_WIDTH 3
#define LANE_COUNT 3
#define LANE_WIDTH (WIDTH / (LANE_COUNT + 1))
#define MAX_OBSTACLES 10
#define INITIAL_LIVES 3
#define BASE_SPEED 80
#define MIN_SPEED 20

int score = 0;
int carLane = 1;  // Start in the middle lane
bool gameOver = false;
int lives = INITIAL_LIVES;
int level = 1;
int speed = BASE_SPEED;

// Obstacle structure
typedef struct {
    int lane;
    float y;
    bool active;
} Obstacle;

Obstacle obstacles[MAX_OBSTACLES];

// Console cursor movement
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
    for (i = 0; i < CAR_WIDTH; i++) {
        gotoxy(x + i, HEIGHT - 2);
        printf("A");
    }
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
            for (j = 0; j < OBSTACLE_WIDTH; j++) {
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
            for (j = 0; j < OBSTACLE_WIDTH; j++) {
                gotoxy(x + j, yPos);
                printf("X");
            }
        }
    }
}

void updateObstacles() {
    int i;
    clearObstacles();

    for (i = 0; i < MAX_OBSTACLES; i++) {
        if (obstacles[i].active) {
            obstacles[i].y += 0.5;

            if (obstacles[i].y >= HEIGHT - 1) {
                obstacles[i].active = false;
                score++;

                // Update level & speed after increasing score
                level = score / 10 + 1;
                speed = BASE_SPEED - (level - 1) * 5;
                if (speed < MIN_SPEED) speed = MIN_SPEED;
            }

            int carX = getLaneX(carLane);
            int obsX = getLaneX(obstacles[i].lane);

            bool xOverlap = (carX < obsX + OBSTACLE_WIDTH) && (carX + CAR_WIDTH > obsX);
            bool yOverlap = ((int)(obstacles[i].y) == HEIGHT - 2);

            if (xOverlap && yOverlap) {
                obstacles[i].active = false;

                lives--;
                if (lives <= 0) {
                    gameOver = true;
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
            break;
        }
    }
}

void drawUI() {
    gotoxy(WIDTH + 5, 2); printf("Score : %d", score);
    gotoxy(WIDTH + 5, 3); printf("Level : %d", level);
    gotoxy(WIDTH + 5, 4); printf("Lives : %d", lives);
    gotoxy(WIDTH + 5, 6); printf("Controls: A/D");
    gotoxy(WIDTH + 5, 7); printf("Exit    : X");
}

void levelUpEffect() {
    int i;
    for (i = 0; i < 3; i++) {
        gotoxy(WIDTH / 2 - 5, HEIGHT / 2);
        printf("LEVEL %d!", level);
        Sleep(200);
        gotoxy(WIDTH / 2 - 5, HEIGHT / 2);
        printf("         ");
        Sleep(200);
    }
}

void gameLoop() {
    int tick = 0;
    int lastLevel = level;
    hideCursor();
    drawBorder();

    while (!gameOver) {
        clearCar();

        if (_kbhit()) {
            char ch = _getch();
            if (ch == 'a' || ch == 'A') {
                if (carLane > 0) carLane--;
            } else if (ch == 'd' || ch == 'D') {
                if (carLane < LANE_COUNT - 1) carLane++;
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

        drawUI();
        drawCar();
        drawObstacles();

        Sleep(speed);
        tick++;
    }

    gotoxy(WIDTH / 2 - 5, HEIGHT / 2);
    printf("GAME OVER!");
    gotoxy(WIDTH / 2 - 7, HEIGHT / 2 + 1);
    printf("FINAL SCORE: %d", score);

    gotoxy(0, HEIGHT + 2);
}

int main() {
    srand(time(0));
    gameLoop();
    return 0;
}

