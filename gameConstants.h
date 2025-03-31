#ifndef GAMECONSTANTS_H
#define GAMECONSTANTS_H

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include <stdbool.h>

//Screen dimensions
#define WIDTH 60
#define HEIGHT 25
#define SCREEN_SIZE (WIDTH * HEIGHT)

//Vehicle dimentsions
#define CAR_WIDTH 11
#define OBSTACLE_WIDTH 5
#define BOSS_WIDTH 11

//Lane config
#define LANE_COUNT 4
#define LANE_WIDTH (WIDTH / (LANE_COUNT + 1))

//Game settings
#define INITIAL_LIVES 3
#define BASE_SPEED 80
#define MIN_SPEED 20

//Powerup settings
#define SLOWMO_DURATION 50
#define OBSTACLE_LIMIT 6 // VISUAL LIMIT OF NUMBER OF OBSTACLES

#endif
