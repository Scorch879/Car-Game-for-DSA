#include "obstacles.h"

ObstacleNode* obstacleHead = NULL;

void spawnObstacle() {
	if (!allLessThanSix(obsCount)) return;
    if(spawn <= maxSpawnsPerTicks)
    {
    	bool powerShield = (rand() % 3 == 0);
    	bool powerSloMo = (rand() % 5 == 0);
    	bool boss = (level % 5 == 0 && rand() % 3 == 0);
    	int lane = rand() % LANE_COUNT;
    	obsCount[lane]++;
    	spawn++;
		addObstacle(lane, boss, powerShield, powerSloMo);
	}
}

bool allLessThanSix(int obsCount[LANE_COUNT]) {
	int i;
    for (i = 0; i < LANE_COUNT; i++)
        if (obsCount[i] >= OBSTACLE_LIMIT) return false;
    return true;
}

void addObstacle(int lane, bool boss, bool powerShield, bool powerSloMo) {
    ObstacleNode* newObs = (ObstacleNode*)malloc(sizeof(ObstacleNode));
    newObs->lane = lane;
    newObs->y = 0;
    newObs->boss = boss;
    newObs->powerShield = powerShield;
    newObs->powerSloMo = powerSloMo;
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
        setColor(current->powerShield ? 11 : 14);
        setColor(current->powerSloMo ? 13 : 14);
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
        	if(current->powerShield || current->powerSloMo){
        		if(current->powerSloMo){
        			slowmo = true;
        			slowmoTicks = SLOWMO_DURATION;
				}
				else{
					shield = true;
				}
			}
			else{
				if (shield) {
                	shield = false;
            	} else {
                	explosionEffect(carX, HEIGHT - 4);
                	lives--;
                	if (lives <= 0) gameOver = true;
            	}
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
            spawn--;
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
    
    if (speed < MIN_SPEED) 
		speed = MIN_SPEED;
}
