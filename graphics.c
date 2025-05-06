#include "graphics.h"
#include "FileHandling.h"
#include "Menu.h"

HANDLE hConsole;

void setColor(int color) 
{
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

void gotoxy(int x, int y) 
{
    COORD coord = {x, y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void hideCursor() {
    CONSOLE_CURSOR_INFO cursorInfo = {100, FALSE};
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

void drawBorder() 
{
    setColor(7);
    for (i = 0; i <= HEIGHT; i++) 
	{
        gotoxy(BORDER_LEFT, i); printf("|");
        gotoxy(BORDER_RIGHT, i); printf("|");
	}
}

void drawCar() 
{
    int x = getLaneX(carLane);
    if(shield)
    	setColor(11);
    else if(slowmo)
    	setColor(13);
    else
    	setColor(10);
    
    gotoxy(x, carY - 2);  printf("   _____   ");
    gotoxy(x, carY - 1);  printf(" _/     \\_ ");
    gotoxy(x, carY);      printf("| 00   00 |");
    gotoxy(x, carY + 1);  printf("""-----------""");
    
    setColor(7);
}

void drawUI() 
{
    gotoxy(BORDER_RIGHT + 5, 2); setColor(15); printf("Score : %d", score);
    gotoxy(BORDER_RIGHT + 5, 3); printf("Level : %d", level);
    gotoxy(BORDER_RIGHT + 5, 4); setColor(lives == 1 ? 12 : 15); printf("Lives : %d", lives);
    gotoxy(BORDER_RIGHT + 5, 5); printf("High Score: %d", highScore);
    gotoxy(BORDER_RIGHT + 5, 7); setColor(powerUpMoves == 1 ? 12 : 15); printf("Power Up Moves Left:    "); // Clear area first
    gotoxy(BORDER_RIGHT + 5, 7); printf("Power Up Moves Left: %d", powerUpMoves);
    gotoxy(BORDER_RIGHT + 5, 8); setColor(bombAmmo < 5 ? 12 : 15); printf("Bomb Ammo Left:   ");
    gotoxy(BORDER_RIGHT + 5, 8); printf("Bomb Ammo Left: %d", bombAmmo);
    gotoxy(BORDER_RIGHT + 5, 9); setColor(coins <= 10 ? 12 : 15);printf("Coins:   ");
    gotoxy(BORDER_RIGHT + 5, 9); printf("Coins: %d", coins);
    gotoxy(BORDER_RIGHT + 5, 10); setColor(shield ? 11 : 8); printf("Shield : %s", shield ? "ON " : "OFF");
    gotoxy(BORDER_RIGHT + 5, 11); setColor(slowmo ? 13 : 8); printf("SlowMo : %s", slowmo ? "ON " : "OFF");
    setColor(15);
    gotoxy(BORDER_RIGHT + 5, 13); printf("Controls:");
    gotoxy(BORDER_RIGHT + 5, 14); printf("W - Forward");
    gotoxy(BORDER_RIGHT + 5, 15); printf("A - Left");
    gotoxy(BORDER_RIGHT + 5, 16); printf("S - Backward");
    gotoxy(BORDER_RIGHT + 5, 17); printf("D - Right");
    gotoxy(BORDER_RIGHT + 5, 20); printf("Shield : Spacebar");
    gotoxy(BORDER_RIGHT + 5, 21); printf("Slowmo : H");
    gotoxy(BORDER_RIGHT + 5, 22); printf("Bomb : B");
    gotoxy(BORDER_RIGHT + 5, 23); printf("Buy Ammo for 10 coins : R");
    gotoxy(BORDER_RIGHT + 5, 24); printf("Exit    : X");
}

void drawLaneMarkers(int tick) 
{
    int laneX;
    setColor(8);
    for (i = 1; i < HEIGHT; i++) 
	{
        if ((i + tick / 2) % 5 == 0) 
		{
            for (l = 0; l < LANE_COUNT; l++) { // FIXED LANE MARKER TO LANE_COUNT INSTEAD OF LANE_COUNT - 1
                laneX = (l + 1) * LANE_WIDTH + BORDER_LEFT; 
                gotoxy(laneX, i);
                printf("|");
            }
        }
    }
    setColor(7);
}

void clearCar() 
{
    int x = getLaneX(carLane);
    for (i = -2; i <= 1; i++) 
	{
        gotoxy(x, carY + i);
        printf("            "); 
    }
}

void explosionEffect(int x, int y) 
{
    setColor(12);
    for (i = 0; i < 3; i++) {
        gotoxy(x - 1, y);     printf(" * ");
        gotoxy(x - 1, y + 1); printf("*X*");
        gotoxy(x - 1, y + 2); printf(" * ");
        Sleep(100);
        gotoxy(x - 1, y);     printf("   ");
        gotoxy(x - 1, y + 1); printf("   ");
        gotoxy(x - 1, y + 2); printf("   ");
        Sleep(100);
    }
    setColor(7);
}

void levelUpEffect() 
{
    setColor(10);
    for (i = 0; i < 3; i++) {
        gotoxy((BORDER_RIGHT + BORDER_LEFT)/2 - 5, HEIGHT / 2);
        printf("LEVEL %d!", level);
        Sleep(200);
        gotoxy((BORDER_RIGHT + BORDER_LEFT)/2 - 5, HEIGHT / 2);
        printf("         ");
        Sleep(200);
    }
    setColor(7);
}

void gameOverScreen() 
{
    system("cls");
    setColor(12);
    
    gotoxy((BORDER_RIGHT + BORDER_LEFT)/2 - 3, HEIGHT / 2 - 2); 
    printf("GAME OVER!");
    
    gotoxy((BORDER_RIGHT + BORDER_LEFT)/2 - 30, HEIGHT / 2 - 1); 
    fflush(stdin);
    
    EntryScore CurrentScore; // Declare EntryScore
    printf("Please Enter your name to save your score (Score won't save if blank): ");
    fgets(CurrentScore.name, sizeof(CurrentScore.name), stdin);
	CurrentScore.name[strcspn(CurrentScore.name, "\n")] = '\0'; // Remove newline
	
   
    trim(CurrentScore.name); // Remove leading/trailing whitespace
    
    if (strlen(CurrentScore.name) != 0) 
    {
    	CurrentScore.score = score; // Store current score
    	saveHighScore(filename, CurrentScore); // Save score and update leaderboard
	}//else it does not save your score if its higher than the last
    
    gotoxy((BORDER_RIGHT + BORDER_LEFT)/2 - 5, HEIGHT / 2 + 1); 
    printf("Score     : %d", score);
    
    gotoxy((BORDER_RIGHT + BORDER_LEFT)/2 - 5, HEIGHT / 2 + 2); 
    printf("High Score: %d", highScore);
    
    gotoxy((BORDER_RIGHT + BORDER_LEFT)/2 - 15, HEIGHT / 2 + 3); 
    printf("Press 'R' to Restart or 'X' to Exit");
    
    setColor(7);
    char choice;
    while (1) 
	{
        choice = _getch();
        if (choice == 'r' || choice == 'R') 
        {
            restartGame();
            break;
        } 
        else if (choice == 'x' || choice == 'X') 
        {
        	gotoxy(0,0);
        	resetGame();
           	mainMenu();
         	return;
        }
    }
}
