#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "game.h"

extern HANDLE hConsole; //Handles colors

void setColor(int color); // sets the colors
void gotoxy(int x, int y); //this is for drawing the UI properly in the console using x and y coords
void hideCursor(); // to hide the blinking cursor in the console
void drawBorder(); // THis is to draw the border around the game
void drawCar(); //to draw the car UI
void clearCar(); //in order to update the position of the car and prevent duplicates
void drawLaneMarkers(int tick); //drawing the lane markers in the console
void drawUI(); //main function to draw the entire ui of the game

// Helper function to trim whitespace
void trim(char *str) 
{
    char *start = str;
    while (isspace(*start)) start++;

    char *end = start + strlen(start) - 1;
    while (end > start && isspace(*end)) end--;
    *(end + 1) = '\0';

    memmove(str, start, strlen(start) + 1);
}

#endif
