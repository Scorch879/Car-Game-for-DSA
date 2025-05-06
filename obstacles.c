#include "obstacles.h"
#include "game.h"

ObstacleNode* obstacleHead = NULL;

void spawnObstacles() 
{
	if (totalObstacleCount >= getMaxObstaclesForLevel(level)) return;

    int availableLanes[LANE_COUNT] = {0, 1, 2, 3};
    int i, j, temp;

    // Shuffle lane order
    for (i = LANE_COUNT - 1; i > 0; i--) {
        j = rand() % (i + 1);
        temp = availableLanes[i];
        availableLanes[i] = availableLanes[j];
        availableLanes[j] = temp;
    }
    
    int lanesToSpawn = 1 + rand() % 3;
	int lanesSpawned = 0;
    int spawnAttempts = 0;

    for (i = 0; i < LANE_COUNT && spawnAttempts < maxSpawnsPerTicks && lanesSpawned < lanesToSpawn; i++) {
        int lane = availableLanes[i];

        if (obsCount[lane] >= OBSTACLE_LIMIT) continue;

        bool powerUp = (rand() % 5 == 0);
        bool boss = (level % 5 == 0 && rand() % 4 == 0);
        bool coins = (rand() % 2 == 0);

        // Create a mock for overlap check
        ObstacleNode test = { lane, 0, boss, powerUp, coins, NULL };
        if (isObstacleOverlapping(&test)) continue;

        // If non-powerup, check if we hit the obstacle cap
        if (!powerUp && totalObstacleCount >= getMaxObstaclesForLevel(level)) continue;

        // Spawn it
        addObstacle(lane, boss, powerUp, coins);

        if (!powerUp && !coins) {
            obsCount[lane]++;
            totalObstacleCount++;  // Count only non-powerUps
        }

        spawnAttempts++;
    }
}

void addObstacle(int lane, bool boss, bool powerUp, bool isCoins) {
    ObstacleNode* newObs = (ObstacleNode*)malloc(sizeof(ObstacleNode));
    newObs->lane = lane;
    newObs->y = 0;
    newObs->boss = boss;
    newObs->powerUp = powerUp;
    newObs->isCoins = isCoins;
    newObs->next = obstacleHead;
    obstacleHead = newObs;
}

void clearObstacles() {
    ObstacleNode* current = obstacleHead;
    int i, j;
    
    while (current != NULL) 
	{
        int x = getLaneX(current->lane);
        int yPos = (int)(current->y);

        if (current->powerUp && !current->boss) 
		{
            // Clear the Power Ups
            for (i = 0; i < 5; i++) 
			{
                int spaces = abs(2 - i);
                int o_count = 5 - 2 * spaces;
                gotoxy(x + spaces, yPos + i);
                for (j = 0; j < o_count; j++) printf(" ");
            }
        } 
		else 
		{
            // Clear the regular/boss obstacle
            for (i = 0; i < 4; i++) 
			{
                gotoxy(x, yPos + i);
                printf("            "); // 12 spaces is safe buffer
            }
        }

        current = current->next;
    }
}

bool isObstacleOverlapping(ObstacleNode* target) {
    int x = getLaneX(target->lane);
    int width = target->boss ? BOSS_WIDTH : OBSTACLE_WIDTH;
    int height = (target->powerUp && !target->boss) ? 5 : 4;
    int top = (int)(target->y);
    int bottom = top + height - 1;

    ObstacleNode* temp = obstacleHead;
    while (temp != NULL) 
	{
        if (temp == target) 
		{
            temp = temp->next;
            continue;
        }

        int tempX = getLaneX(temp->lane);
        int tempWidth = temp->boss ? BOSS_WIDTH : OBSTACLE_WIDTH;
        int tempHeight = (temp->powerUp && !temp->boss) ? 5 : 4;
        int tempTop = (int)(temp->y);
        int tempBottom = tempTop + tempHeight - 1;

        bool xOverlap = (x < tempX + tempWidth) && (x + width > tempX);
        bool yOverlap = !(bottom < tempTop || top > tempBottom);

        if (xOverlap && yOverlap) 
		{
            return true;
        }

        temp = temp->next;
    }

    return false;
}

void drawObstacles() {
    ObstacleNode* current = obstacleHead, *prev = NULL;
    while (current != NULL) 
	{
        if (isObstacleOverlapping(current)) 
		{
            if (obsCount[current->lane] > 0) obsCount[current->lane]--;
            if (totalObstacleCount > 0) totalObstacleCount--;

            if (prev == NULL) 
			{
                obstacleHead = current->next;
                free(current);
                current = obstacleHead;
            } 
			else 
			{
                prev->next = current->next;
                free(current);
                current = prev->next;
            }
            continue;
        }

        // Drawing logic
        int x = getLaneX(current->lane);
        int yPos = (int)(current->y);
        int i, j;

        if (current->powerUp && !current->boss) 
		{
            setColor(1); // Cyan
            for (i = 0; i < 5; i++) 
			{
                int spaces = abs(2 - i);
                int o_count = 5 - 2 * spaces;
                gotoxy(x + spaces, yPos + i);
                for (j = 0; j < o_count; j++) printf("o");
            }
        }
		else if (current->isCoins && (!current->boss && !current->powerUp)) 
		{
            setColor(6); 
			for (i = 0; i < 3; i++) {
			    int o_count = 5 - 2 * i;
			    int spaces = (5 - o_count) / 2; 
			    gotoxy(x + spaces + 3, yPos + i);
			    for (j = 0; j < o_count; j++) {
			        printf("o");
			    }
			}
        }  
		else 
		{
            setColor(current->boss ? 12 : 14);
            gotoxy(x, yPos);     printf("   _____   ");
            gotoxy(x, yPos + 1); printf(" /  ___  \\ ");
            gotoxy(x, yPos + 2); printf("|[o]___[o]|");
            gotoxy(x, yPos + 3); printf(" --------- ");
        }

        prev = current;
        current = current->next;
    }

    setColor(7); // Reset color
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
        bool yOverlap = ((int)(current->y + 3) >= carY - 2 && (int)(current->y) <= carY + 1);

        if (xOverlap && yOverlap) 
		{
            if (current->powerUp) {
                powerUpMoves++;
            }
			else if(current->isCoins){
				coins++;
			} 
			else {
            	if (totalObstacleCount > 0) totalObstacleCount--;
                if (shield) {
                    shield = false;
                } else {
                    explosionEffect(carX, carY);
                    if(current->boss)
                    {
                    	lives--;
                    	lives--;
                    }
                    else
                    	lives--;
                    if (lives <= 0) gameOver = true;
                }
            }
            
    		if (obsCount[current->lane] > 0) obsCount[current->lane]--;
    		
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
        
        if(carX == obsX)
        {
        	if (bombCar) {
			    bombCar = false;
			
			    ObstacleNode* temp = obstacleHead;
			    ObstacleNode* prevTemp = NULL;
			    ObstacleNode* closest = NULL;
			    ObstacleNode* prevClosest = NULL;
			    float maxY = -1;
			
			    while (temp != NULL) {
			        int tempX = getLaneX(temp->lane);
			        if (
			            tempX == getLaneX(carLane) &&
			            !temp->powerUp &&
			            !temp->isCoins &&
			            temp->y < carY && // must be in front of the car
			            temp->y > maxY    // highest y still less than carY
			        ) {
			            maxY = temp->y;
			            closest = temp;
			            prevClosest = prevTemp;
			        }
			        prevTemp = temp;
			        temp = temp->next;
			    }
			
			    if (closest != NULL) {
			        explosionEffect(getLaneX(closest->lane), (int)closest->y);
			        score++;
			        if (obsCount[closest->lane] > 0) obsCount[closest->lane]--;
			        if (totalObstacleCount > 0) totalObstacleCount--;
			
			        if (prevClosest == NULL) {
			            obstacleHead = closest->next;
			        } else {
			            prevClosest->next = closest->next;
			        }
			        free(closest);
			    }
			}
		}

        if ((int)(current->y) > HEIGHT) {
            score++;
            if (obsCount[current->lane] > 0) obsCount[current->lane]--;
            if (totalObstacleCount > 0) totalObstacleCount--;

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
}
