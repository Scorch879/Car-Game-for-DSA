#ifndef OBSTACLES_H
#define OBSTACLES_H

#include "game.h"

typedef struct ObstacleNode {
    int lane;
    float y;
    bool boss;
    bool powerShield;
    bool powerSloMo;
    struct ObstacleNode* next;
} ObstacleNode;

extern ObstacleNode* obstacleHead;

bool allLessThanSix(int obsCount[LANE_COUNT]);
void addObstacle(int lane, bool boss, bool powerUp, bool powerSloMo);
void clearObstacles();
void drawObstacles();
void updateObstacles();
void spawnObstacle();

#endif
