#ifndef OBSTACLES_H
#define OBSTACLES_H

#include "game.h"

typedef struct ObstacleNode {
    int lane;
    float y;
    bool boss;
    struct ObstacleNode* next;
} ObstacleNode;

extern ObstacleNode* obstacleHead;

bool allLessThanSix(int obsCount[LANE_COUNT]);
void addObstacle(int lane, bool boss);
void clearObstacles();
void drawObstacles();
void updateObstacles();
void spawnObstacle();

#endif
