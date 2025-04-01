#ifndef MENU_H
#define MENU_H

#include "game.h"
#include "graphics.h"

#define MENU_SIZE 3

extern const char *menuOptions[MENU_SIZE];
extern int selectedOption;

void mainMenu();
void displayMenu();

#endif
