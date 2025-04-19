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
bool shield = false;
bool slowmo = false;
int slowmoTicks = 0;
int powerUpMoves = 0;
int maxSpawnsPerTicks = 0;
int highScore = 0;

int i = 0, j = 0, l = 0;

void moveCarForward() {
    // MOVES VAR FORWARD
    carY -= 1;

    // FORWARD MOVEMENT LIMIT
    if (carY < HEIGHT - 20) {
        carY = HEIGHT - 20;
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
    powerUpMoves = 0;
	maxSpawnsPerTicks = 0;

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
    shield = false;
    slowmo = false;
    slowmoTicks = 0;
    powerUpMoves = 0;
	maxSpawnsPerTicks = 0;

    system("cls");
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
                activatePowerUp();
            } 
			else if (ch == 'x' || ch == 'X') 
			{
                gameOver = true;
            }
        }

        if (tick % 2 == 0) {
            updateObstacles();
			  // 1. Adjust spawn rate based on level (lower = more frequent)
		    int spawnRate = (level % 5 == 0) ? 1 : 6 - (level % 5);
		
		    // 2. Control max spawns per tick: increases every 5 levels, then restarts
		    int difficultyStage = (level - 1) / 5;  // 0 for levels 1–5, 1 for 6–10, etc.
		    maxSpawnsPerTicks = 2 + (difficultyStage % 3);  // 2 to 4, then loop back
		    
			if (maxSpawnsPerTicks > 4) maxSpawnsPerTicks = 4;
            if (rand() % spawnRate == 0) {
            	spawnObstacles();
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

//    saveHighScore();
    gameOverScreen();
    
    
}
