#include "game.h"
#include "gameConstants.h"
#include "graphics.h"
#include "highscore.h"
#include "obstacles.h"
#include "powerups.h"

int main() {
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    srand((unsigned int)time(NULL));
    system("cls");
    gameLoop();
    return 0;
}
