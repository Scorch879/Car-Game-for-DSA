#ifndef OBSTACLES_H
#define OBSTACLES_H

#include "game.h"

typedef struct ObstacleNode {
    int lane;
    float y;
    bool boss;
    bool powerUp;
    bool isCoins;
    struct ObstacleNode* next;
} ObstacleNode;

extern ObstacleNode* obstacleHead;

void addObstacle(int lane, bool boss, bool powerUp, bool isCoins);
void clearObstacles();
bool isObstacleOverlapping(ObstacleNode* target);
void drawObstacles();
void updateObstacles();
void spawnObstacle();

#endif
