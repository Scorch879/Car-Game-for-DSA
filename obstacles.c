#include "obstacles.h"

ObstacleNode* obstacleHead = NULL;


void spawnObstacles() 
{
    if (!allLessThanSix(obsCount)) return;

    int availableLanes[LANE_COUNT] = {0, 1, 2, 3};
    int i, j, temp, spawn = 0;

    // Shuffle lane order
    for (i = LANE_COUNT - 1; i > 0; i--) {
        j = rand() % (i + 1);
        temp = availableLanes[i];
        availableLanes[i] = availableLanes[j];
        availableLanes[j] = temp;
    }

    int lanesToSpawn = 1 + rand() % 3; // Randomly spawn in 1 to 3 lanes
    int lanesSpawned = 0;

    for (i = 0; i < LANE_COUNT && lanesSpawned < lanesToSpawn && spawn < maxSpawnsPerTicks; i++) {
        int lane = availableLanes[i];

        if (obsCount[lane] < OBSTACLE_LIMIT) {
            bool powerUp = (rand() % 5 == 0);
            bool boss = (level % 5 == 0 && rand() % 4 == 0);

            obsCount[lane]++;
            spawn++;
            addObstacle(lane, boss, powerUp);
            lanesSpawned++;
        }
    }
}

bool allLessThanSix(int obsCount[LANE_COUNT]) {
    int i;
    for (i = 0; i < LANE_COUNT; i++) {
        if (obsCount[i] >= OBSTACLE_LIMIT) return false;
    }
    return true;
}

void addObstacle(int lane, bool boss, bool powerUp) {
    ObstacleNode* newObs = (ObstacleNode*)malloc(sizeof(ObstacleNode));
    newObs->lane = lane;
    newObs->y = 0;
    newObs->boss = boss;
    newObs->powerUp = powerUp;
    newObs->next = obstacleHead;
    obstacleHead = newObs;
}

void clearObstacles() {
    ObstacleNode* current = obstacleHead;
    while (current != NULL) {
        int x = getLaneX(current->lane);
        int yPos = (int)(current->y);
        int width = current->boss ? BOSS_WIDTH : OBSTACLE_WIDTH;

        int j;
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

        if (current->powerUp && !current->boss)
            setColor(11);
        else if (current->boss)
            setColor(12);
        else
            setColor(14);

        int j;
        for (j = 0; j < width; j++) {
            gotoxy(x + j, yPos);
            printf("%c", current->boss ? '#' : 'X');
        }

        current = current->next;
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
            if (current->powerUp) {
                powerUpMoves++;
            } else {
                if (shield) {
                    shield = false;
                } else {
                    explosionEffect(carX, HEIGHT - 4);
                    lives--;
                    if (lives <= 0) gameOver = true;
                }
            }
            
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

    level = score / 20 + 1;
    speed = BASE_SPEED - (level - 1) * 5;
    if (speed < MIN_SPEED) speed = MIN_SPEED;
}
