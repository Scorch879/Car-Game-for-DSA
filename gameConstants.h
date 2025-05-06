#ifndef GAMECONSTANTS_H
#define GAMECONSTANTS_H

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include <stdbool.h>
#include <conio.h>

//Screen dimensions
#define WIDTH 70
#define HEIGHT 44

//Vehicle dimentsions
#define CAR_WIDTH 11
#define OBSTACLE_WIDTH 5
#define BOSS_WIDTH 11

//Lane config
#define LANE_COUNT 4
#define LANE_WIDTH (WIDTH / (LANE_COUNT + 1))
#define BORDER_LEFT (LANE_WIDTH * 3)
#define BORDER_RIGHT (WIDTH + BORDER_LEFT)

//Game settings
#define INITIAL_LIVES 5
#define BASE_SPEED 80
#define MIN_SPEED 20

//Powerup settings
#define BOMB_PRICE 10
#define BOMB_DURATION 20
#define SLOWMO_DURATION 50
#define OBSTACLE_LIMIT 4 // VISUAL LIMIT OF NUMBER OF OBSTACLES
#define OBSTACLE_COUNT 60

#endif
