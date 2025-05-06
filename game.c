#include "game.h"
#include "obstacles.h"
#include "powerups.h"
#include "FileHandling.h"

//initialization of extern variables
int score = 0;
int obsCount[LANE_COUNT] = {0};
int carLane = 1;
int carY = HEIGHT;
bool gameOver = false;
int lives = INITIAL_LIVES;
int level = 1;
int speed = BASE_SPEED;
int bombAmmo = 0;
bool bombCar = 0;
int bombTicks = 0;
bool shield = false;
bool slowmo = false;
int slowmoTicks = 0;
int powerUpMoves = 0;
int coins = 0;
int maxSpawnsPerTicks = 0;
int totalObstacleCount = 0;
int highScore = 0;

int i = 0, j = 0, l = 0;

void moveCarForward() {
    // MOVES VAR FORWARD
    carY -= 1;

    // FORWARD MOVEMENT LIMIT
    if (carY < HEIGHT - 30) {
        carY = HEIGHT - 30;
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
    int laneCenter = (lane + 1) * LANE_WIDTH + BORDER_LEFT;
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
    bombAmmo = 0;
	bombCar = 0;
	bombTicks = 0;
    shield = false;
    slowmo = false;
    slowmoTicks = 0;
    powerUpMoves = 0;
    coins = 0;
	maxSpawnsPerTicks = 0;
	totalObstacleCount = 0;

    system("cls");
    gameLoop();
}

void resetGame()
{
	ObstacleNode* current = obstacleHead;
    while (current != NULL) 
	{
        ObstacleNode* temp = current;
        current = current->next;
        free(temp);
    }
    obstacleHead = NULL;
	
	carY = HEIGHT;
    score = 0;
    memset(obsCount, 0, sizeof(obsCount));
    carLane = 1;
    gameOver = false;
    lives = INITIAL_LIVES;
    level = 1;
    speed = BASE_SPEED;
    bombAmmo = 0;
	bombCar = 0;
	bombTicks = 0;
    shield = false;
    slowmo = false;
    slowmoTicks = 0;
    powerUpMoves = 0;
    coins = 0;
	maxSpawnsPerTicks = 0;
	totalObstacleCount = 0;

    system("cls");
}

int getMaxObstaclesForLevel(int level) {
    if (level <= 5) return 4;
    if (level <= 10) return 5;
    if (level <= 15) return 6;
    return 6;
}

void gameLoop() {
	
    int tick = 0;
    int lastLevel = level;

    hideCursor();
    highScore = loadHighestScore(filename);
    drawBorder();

    while (!gameOver) {
        clearCar();

        if (_kbhit()) {
            char ch = _getch();
            if (ch == 'a' || ch == 'A') 
			{
                if (carLane > 0) carLane--;
            } 
			else if (ch == 'd' || ch == 'D') 
			{
                if (carLane < LANE_COUNT - 1) carLane++;
            } 
			else if (ch == 'w' || ch == 'W') 
			{
        		moveCarForward();
			} 
			else if (ch == 's' || ch == 'S') 
			{
        		moveCarBackward();
            } 
			else if (ch == ' ' && powerUpMoves != 0) 
			{
				powerUpMoves--;
                activateShield();
            }
			else if((ch == 'h' || ch == 'H') && powerUpMoves != 0)
			{
				powerUpMoves--;
				activateSlowmo();
			}
			else if ((ch == 'r' || ch == 'R') && coins >= BOMB_PRICE)
			{
				coins -= BOMB_PRICE;
				reloadBomb();
			}
			else if ((ch == 'b' || ch == 'B') && bombAmmo != 0)
			{
				bombAmmo--;
				activateBomb();
			}
			else if (ch == 'x' || ch == 'X') 
			{
                gameOver = true;
            }
        }
        
        level = score / 20 + 1;
    	speed = BASE_SPEED - (level - 1) * 5;
    	if (speed < MIN_SPEED) speed = MIN_SPEED;
    	
    	if (level > lastLevel) {
            levelUpEffect();
            lastLevel = level;
        }

        if (tick % 2 == 0) {
            updateObstacles();
			  // 1. Adjust spawn rate based on level (lower = more frequent)
		    int spawnRate = (level % 5 == 0) ? 1 : 6 - (level % 5);
		    
		    int difficultyLevel = (level - 1) / 5;
		    maxSpawnsPerTicks = 1 + (difficultyLevel % 3);
		    
            if (rand() % spawnRate == 0 && totalObstacleCount < getMaxObstaclesForLevel(level)) {
            	spawnObstacles();
            }
        }

        if (slowmo && --slowmoTicks <= 0) {
            slowmo = false;
        }
        
        if (bombCar && --bombTicks <= 0) {
            bombCar = false;
        }

        drawLaneMarkers(tick);
        drawUI();
        drawCar();
        drawObstacles();

        Sleep(speed);
        tick++;
    }
    
	freeAllObstacles();
    gameOverScreen();
    
}

void freeAllObstacles() {
    ObstacleNode* current = obstacleHead;
    while (current) {
        ObstacleNode* temp = current;
        current = current->next;
        free(temp);
    }
    obstacleHead = NULL;
}
