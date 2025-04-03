#include "graphics.h"
#include "FileHandling.h"

void setColor(int color) 
{
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
        gotoxy(0, i); printf("|");
        gotoxy(WIDTH, i); printf("|");
    
	}
}

void drawCar() 
{
    int x = getLaneX(carLane);
    setColor(shield ? 11 : 10);
    
    gotoxy(x, carY - 2);  printf("   _____   ");
    gotoxy(x, carY - 1);  printf(" _/     \\_ ");
    gotoxy(x, carY);      printf("| 00   00 |");
    gotoxy(x, carY + 1);  printf("""-----------""");
    
    setColor(7);
}

void drawUI() 
{
    gotoxy(WIDTH + 5, 2); setColor(15); printf("Score : %d", score);
    gotoxy(WIDTH + 5, 3); printf("Level : %d", level);
    gotoxy(WIDTH + 5, 4); setColor(lives == 1 ? 12 : 15); printf("Lives : %d", lives);
    gotoxy(WIDTH + 5, 5); printf("High Score: %d", highScore);
    gotoxy(WIDTH + 5, 7); setColor(shield ? 11 : 8); printf("Shield : %s", shield ? "ON " : "OFF");
    gotoxy(WIDTH + 5, 8); setColor(slowmo ? 13 : 8); printf("SlowMo : %s", slowmo ? "ON " : "OFF");
    setColor(15);
    gotoxy(WIDTH + 5, 10); printf("Controls:");
    gotoxy(WIDTH + 5, 11); printf("W - Forward");
    gotoxy(WIDTH + 5, 12); printf("A - Left");
    gotoxy(WIDTH + 5, 13); printf("S - Backward");
    gotoxy(WIDTH + 5, 14); printf("D - Right");
    gotoxy(WIDTH + 5, 16); printf("PowerUp : Z");
    gotoxy(WIDTH + 5, 17); printf("Exit    : X");
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
                laneX = (l + 1) * LANE_WIDTH; 
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
        gotoxy(WIDTH / 2 - 5, HEIGHT / 2);
        printf("LEVEL %d!", level);
        Sleep(200);
        gotoxy(WIDTH / 2 - 5, HEIGHT / 2);
        printf("         ");
        Sleep(200);
    }
    setColor(7);
}

void gameOverScreen() 
{
    system("cls");
    setColor(12);
    
    gotoxy(WIDTH / 2 - 3, HEIGHT / 2 - 2); 
    printf("GAME OVER!");
     
    gotoxy(WIDTH / 2 - 30, HEIGHT / 2 - 1); 
    printf("Please Enter your name to save your score (Score won't save if blank): ");
    
    EntryScore CurrentScore; // Declare EntryScore
    scanf("%[^\n]", CurrentScore.name); // Read name (max 49 characters)
    
    if (strlen(CurrentScore.name) != 0) 
    {
    	CurrentScore.score = score; // Store current score
    	saveHighScore(filename, CurrentScore); // Save score and update leaderboard
	}
    
    gotoxy(WIDTH / 2 - 5, HEIGHT / 2 + 1); 
    printf("Score     : %d", score);
    
    gotoxy(WIDTH / 2 - 5, HEIGHT / 2 + 2); 
    printf("High Score: %d", highScore);
    
    gotoxy(WIDTH / 2 - 15, HEIGHT / 2 + 3); 
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
            resetGame();
            mainMenu();
        }
    }
}
