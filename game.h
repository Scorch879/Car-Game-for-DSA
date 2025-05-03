#ifndef GAME_H
#define GAME_H

#include "gameConstants.h"

//Global game variables
extern int score;
extern int obsCount[LANE_COUNT];
extern int carLane ;
extern int carY;
extern bool gameOver ;
extern int lives;
extern int level;
extern int speed;
extern bool shield;
extern bool slowmo;
extern int slowmoTicks;
extern int powerUpMoves;
extern int maxSpawnsPerTicks;
extern int totalObstacleCount;
extern int highScore;

int i, j, l;

void moveCarForward();
void moveCarBackward();
int getLaneX(int lane);
void restartGame();
void resetGame();
int getMaxObstaclesForLevel(int level);
void gameLoop();

#endif
