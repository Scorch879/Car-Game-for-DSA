#include "game.h"
#include "obstacles.h"

//declaration of extern variables
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

int i = 0, j = 0, l = 0;

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

int getLaneX(int lane) {
    int laneCenter = (lane + 1) * LANE_WIDTH;
    return laneCenter - (CAR_WIDTH / 2);
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
