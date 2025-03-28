#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <stdbool.h>
#include <windows.h>
#include <time.h>

// Game constants
#define WIDTH 60
#define HEIGHT 25
#define SCREEN_SIZE (WIDTH * HEIGHT)
#define CAR_WIDTH 11
#define OBSTACLE_WIDTH 5
#define BOSS_WIDTH 11
#define LANE_COUNT 4
#define LANE_WIDTH (WIDTH / (LANE_COUNT + 1))
#define INITIAL_LIVES 3
#define BASE_SPEED 80
#define MIN_SPEED 20
#define SLOWMO_DURATION 50
#define OBSTACLE_LIMIT 6 // VISUAL LIMIT OF NUMBER OF OBSTACLES

//Global game variables
int score = 0;
int obsCount[LANE_COUNT] = {0};
int carLane = 1;
int carY = HEIGHT;
bool gameOver = false;
int lives = INITIAL_LIVES;
int level = 1;
int speed = BASE_SPEED;
bool shield = false;
bool slowmo = false;
int slowmoTicks = 0;
int highScore = 0;


int i, j, l;

HANDLE hConsole; //Handles colors


typedef struct ObstacleNode {
    int lane;
    float y;
    bool boss;
    struct ObstacleNode* next;
} ObstacleNode;

ObstacleNode* obstacleHead = NULL;


void restartGame();
void gameLoop();

void setColor(int color) {
    SetConsoleTextAttribute(hConsole, color);
}

void gotoxy(int x, int y) {
    COORD coord = {x, y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void hideCursor() {
    CONSOLE_CURSOR_INFO cursorInfo = {100, FALSE};
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

void drawBorder() {
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
    setColor(shield ? 11 : 10);
    
    gotoxy(x, carY - 2);  printf("   _____   ");
    gotoxy(x, carY - 1);  printf(" _/     \\_ ");
    gotoxy(x, carY);      printf("| 00   00 |");
    gotoxy(x, carY + 1);  printf("""-----------""");
    
    setColor(7);
}

void clearCar() {
    int x = getLaneX(carLane);
    for (i = -2; i <= 1; i++) {
        gotoxy(x, carY + i);
        printf("            "); 
    }
}

void moveCarForward() {
    // MOVES VAR FORWARD
    carY -= 1;

    // FORWARD MOVEMENT LIMIT
    if (carY < HEIGHT - 10) {
        carY = HEIGHT - 10;
    }
}

void moveCarBackward() {
    // MOVES CAR BACKWARD
    carY += 1;
    
    if (carY > HEIGHT) {
        carY = HEIGHT; // PREVENTS FROM GOING BELOW ORIGINAL POSITION
    }
}

void drawLaneMarkers(int tick) {
    int laneX;
    setColor(8);
    for (i = 1; i < HEIGHT; i++) {
        if ((i + tick / 2) % 5 == 0) {
            for (l = 0; l < LANE_COUNT; l++) { // FIXED LANE MARKER TO LANE_COUNT INSTEAD OF LANE_COUNT - 1
                laneX = (l + 1) * LANE_WIDTH; 
                gotoxy(laneX, i);
                printf("|");
            }
        }
    }
    setColor(7);
}

void addObstacle(int lane, bool boss) {
    ObstacleNode* newObs = (ObstacleNode*)malloc(sizeof(ObstacleNode));
    newObs->lane = lane;
    newObs->y = 0;
    newObs->boss = boss;
    newObs->next = obstacleHead;
    obstacleHead = newObs;
}

void clearObstacles() {
    ObstacleNode* current = obstacleHead;
    while (current != NULL) {
        int x = getLaneX(current->lane);
        int yPos = (int)(current->y);
        int width = current->boss ? BOSS_WIDTH : OBSTACLE_WIDTH;
        for (j = 0; j < width; j++) {
            gotoxy(x + j, yPos);
            printf(" ");
        }
        current = current->next;
    }
}

void drawObstacles() {
    ObstacleNode* current = obstacleHead;
    while (current != NULL) {
        int x = getLaneX(current->lane);
        int yPos = (int)(current->y);
        int width = current->boss ? BOSS_WIDTH : OBSTACLE_WIDTH;
        setColor(current->boss ? 12 : 14);
        for (j = 0; j < width; j++) {
            gotoxy(x + j, yPos);
            printf("%c", current->boss ? '#' : 'X');
        }
        current = current->next;
    }
    setColor(7);
}

void explosionEffect(int x, int y) {
    setColor(12);
    for (i = 0; i < 3; i++) {
        gotoxy(x - 1, y);     printf(" * ");
        gotoxy(x - 1, y + 1); printf("*X*");
        gotoxy(x - 1, y + 2); printf(" * ");
        Sleep(100);
        gotoxy(x - 1, y);     printf("   ");
        gotoxy(x - 1, y + 1); printf("   ");
        gotoxy(x - 1, y + 2); printf("   ");
        Sleep(100);
    }
    setColor(7);
}

void updateObstacles() {
    ObstacleNode *current = obstacleHead, *prev = NULL;

    clearObstacles();

    while (current != NULL) {
        current->y += slowmo ? 0.2 : 0.5;

        int carX = getLaneX(carLane);
        int obsX = getLaneX(current->lane);
        int obsWidth = current->boss ? BOSS_WIDTH : OBSTACLE_WIDTH;

        bool xOverlap = (carX < obsX + obsWidth) && (carX + CAR_WIDTH > obsX);
		bool yOverlap = ((int)(current->y) >= carY - 2 && (int)(current->y) <= carY + 1); 

        if (xOverlap && yOverlap) {
            if (shield) {
                shield = false;
            } else {
                explosionEffect(carX, HEIGHT - 4);
                lives--;
                if (lives <= 0) gameOver = true;
            }
            if (prev == NULL) {
                obstacleHead = current->next;
                free(current);
                current = obstacleHead;
            } else {
                prev->next = current->next;
                free(current);
                current = prev->next;
            }
            continue;
        }

        if ((int)(current->y) > HEIGHT) {
            score++;
            obsCount[current->lane]--;
            if (prev == NULL) {
                obstacleHead = current->next;
                free(current);
                current = obstacleHead;
            } else {
                prev->next = current->next;
                free(current);
                current = prev->next;
            }
            continue;
        }

        prev = current;
        current = current->next;
    }

    level = score / 20 + 1; // ADJUSTED THE LEVELING TO EVERY 20 SCORE MARKS
    speed = BASE_SPEED - (level - 1) * 5;
    if (speed < MIN_SPEED) speed = MIN_SPEED;
}

bool allLessThanSix(int obsCount[LANE_COUNT]) {
	int i;
    for (i = 0; i < LANE_COUNT; i++)
        if (obsCount[i] >= OBSTACLE_LIMIT) return false;
    return true;
}

void spawnObstacle() {
	if (!allLessThanSix(obsCount)) return;
    bool boss = (level % 5 == 0 && rand() % 3 == 0);
    int lane = rand() % LANE_COUNT;
    obsCount[lane]++;
	addObstacle(lane, boss);
}

void drawUI() {
    gotoxy(WIDTH + 5, 2); setColor(15); printf("Score : %d", score);
    gotoxy(WIDTH + 5, 3); printf("Level : %d", level);
    gotoxy(WIDTH + 5, 4); setColor(lives == 1 ? 12 : 15); printf("Lives : %d", lives);
    gotoxy(WIDTH + 5, 5); printf("High Score: %d", highScore);
    gotoxy(WIDTH + 5, 7); setColor(shield ? 11 : 8); printf("Shield : %s", shield ? "ON " : "OFF");
    gotoxy(WIDTH + 5, 8); setColor(slowmo ? 13 : 8); printf("SlowMo : %s", slowmo ? "ON " : "OFF");
    setColor(15);
    gotoxy(WIDTH + 5, 10); printf("Controls:");
    gotoxy(WIDTH + 5, 11); printf("W - Forward");
    gotoxy(WIDTH + 5, 12); printf("A - Left");
    gotoxy(WIDTH + 5, 13); printf("S - Backward");
    gotoxy(WIDTH + 5, 14); printf("D - Right");
    gotoxy(WIDTH + 5, 16); printf("PowerUp : Z");
    gotoxy(WIDTH + 5, 17); printf("Exit    : X");
}

void levelUpEffect() {
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
	
	FILE *file = fopen("highscore.txt", "r");
	
	//If the file does not exist
	if (!file) {
		file = fopen("highscore.txt", "w");
		if (!file) {
            printf("\nError: Unable to create highscore file.\n\n");
            return;
        }
        	fprintf(file, "0"); // Initialize with 0 if no previous high score exists
		fclose(file);
	}
		
	else {
		fclose(file);
	}
	
    if (score > highScore) {
        FILE* file = fopen("highscore.txt", "w");
        if (file) {
            fprintf(file, "%d", score);
            fclose(file);
        }
        else {
        	printf("\nError: Unable to create highscore file.\n\n");
		}
    }
}

void loadHighScore() {
    FILE* file = fopen("highscore.txt", "r");
    if (file) {
        fscanf(file, "%d", &highScore);
        fclose(file);
    }
}

void activatePowerUp() {
    int power = rand() % 2;
    if (power == 0) shield = true;
    else {
        slowmo = true;
        slowmoTicks = SLOWMO_DURATION;
    }
}

void gameOverScreen() {
    system("cls");
    setColor(12);
    gotoxy(WIDTH / 2 - 6, HEIGHT / 2 - 2); printf("GAME OVER!");
    gotoxy(WIDTH / 2 - 8, HEIGHT / 2); printf("Score     : %d", score);
    gotoxy(WIDTH / 2 - 8, HEIGHT / 2 + 1); printf("High Score: %d", highScore);
    gotoxy(WIDTH / 2 - 12, HEIGHT / 2 + 3); printf("Press 'R' to Restart or 'X' to Exit");
    setColor(7);

    char choice;
    while (1) {
        choice = _getch();
        if (choice == 'r' || choice == 'R') {
            restartGame();
            break;
        } else if (choice == 'x' || choice == 'X') {
            exit(0);
        }
    }
}

void restartGame() {
    ObstacleNode* current = obstacleHead;
    while (current != NULL) {
        ObstacleNode* temp = current;
        current = current->next;
        free(temp);
    }
    obstacleHead = NULL;

    score = 0;
    memset(obsCount, 0, sizeof(obsCount));
    carLane = 1;
    gameOver = false;
    lives = INITIAL_LIVES;
    level = 1;
    speed = BASE_SPEED;
    shield = false;
    slowmo = false;
    slowmoTicks = 0;

    system("cls");
    gameLoop();
}

void gameLoop() {
    int tick = 0;
    int lastLevel = level;

    hideCursor();
    loadHighScore();
    drawBorder();

    while (!gameOver) {
        clearCar();

        if (_kbhit()) {
            char ch = _getch();
            if (ch == 'a' || ch == 'A') {
                if (carLane > 0) carLane--;
            } else if (ch == 'd' || ch == 'D') {
                if (carLane < LANE_COUNT - 1) carLane++;
            } else if (ch == 'w' || ch == 'W') {
        		moveCarForward();
			} else if (ch == 's' || ch == 'S') {
        		moveCarBackward();
            } else if (ch == 'z' || ch == 'Z') {
                activatePowerUp();
            } else if (ch == 'x' || ch == 'X') {
                gameOver = true;
            }
        }

        if (tick % 2 == 0) {
            updateObstacles();
            int spawnRate = ((5 - level) % 5 == 0) ? 1 : 5 - level; //ADJUSTED THE SPAWNRATE FOR MORE DOABLE LEVELS
            if (rand() % spawnRate == 0) {
            	if(allLessThanSix(obsCount)) //CONTROLS THE NUMBER OBSTACLE SPAWNS PER LANE
            	{
                	spawnObstacle();
                }
            }
        }

        if (level > lastLevel) {
            levelUpEffect();
            lastLevel = level;
        }

        if (slowmo && --slowmoTicks <= 0) {
            slowmo = false;
        }

        drawLaneMarkers(tick);
        drawUI();
        drawCar();
        drawObstacles();

        Sleep(speed);
        tick++;
    }

    saveHighScore();
    gameOverScreen();
}

int main() {
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    srand((unsigned int)time(NULL));
    system("cls");
    gameLoop();
    return 0;
}
