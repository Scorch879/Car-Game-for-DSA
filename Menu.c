#include "Menu.h"
#include "FileHandling.h"

//declaration
const char *menuOptions[MENU_SIZE] = {"Start Game", "Instructions", "Exit"};
int selectedOption = 0;

void displayMenu() 
{
        gotoxy(5, 2);
        setColor(10);
        printf("===== CAR GAME MENU =====");

        gotoxy(5, 12 + MENU_SIZE * 6);
        setColor(8);
        printf("Use W/S or Arrow Keys to navigate, ENTER or SPACE to select.");

        // Draw the scoreboard
        displayHighScores(filename);

    // Redraw only the menu options
    for (i = 0; i < MENU_SIZE; i++) 
	{
        gotoxy(5, 4 + i * 2);
        if (i == selectedOption) 
		{
            setColor(11); 
            printf(" > %s <", menuOptions[i]);
        } 
		else 
		{
            setColor(7);
            printf("   %s   ", menuOptions[i]);
        }
    }

    setColor(7);
}

void mainMenu() 
{
    char key;
    hideCursor();
    int status = 1;
    
    while (status == 1) 
	{
        displayMenu();
        key = getch(); // Get user input

        if (key == 72 || key == 'w') 
		{ // Up arrow or 'W'
            selectedOption = (selectedOption - 1 + MENU_SIZE) % MENU_SIZE;
        } 
		else if (key == 80 || key == 's') 
		{ 
            selectedOption = (selectedOption + 1) % MENU_SIZE;
        } 
		else if (key == 13) 
		{ 
           // gotoxy(0, MENU_SIZE + 3);
        
            if (selectedOption == 0) // Game Start
            {
			    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
			    srand((unsigned int)time(NULL));
			    system("cls");
			    gameLoop();
			}
			else if (selectedOption == 1)
			{
				//Instructions or about idk
				printf("Hi");
			}
			else if (selectedOption == 2) //Exit Game
			{
				system("cls");
				exit(0);
				status == 0;
			} 
				
            system("pause");
            system("cls");
        }
        else if (key == ' ') //for spacebar key press
		{ 
            if (selectedOption == 0) 
			{ // Game Start
                hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
                srand((unsigned int)time(NULL));
                system("cls");
                gameLoop();
            }
            else if (selectedOption == 1) 
			{ 
				//printf("Hi");
				// Instructions
                // Handle Instructions
            }
            else if (selectedOption == 2) 
			{ 
				system("cls");
                exit(0);
            }
        }
    }
}
