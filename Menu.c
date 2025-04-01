#include "Menu.h"

//declaration
const char *menuOptions[MENU_SIZE] = {"Start Game", "Instructions", "Exit"};
int selectedOption = 0;

void displayMenu() {
    gotoxy(0, 0); // Move cursor to top left to overwrite text
    printf("===== CAR GAME MENU =====\n");

    for (i = 0; i < MENU_SIZE; i++) 
	{
        gotoxy(0, i + 2); // Position menu items below title
        if (i == selectedOption) 
		{
            printf(" > %s <   ", menuOptions[i]); // Highlight selected option
        } 
		else 
		{
            printf("   %s     ", menuOptions[i]);
        }
    }
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

        if (key == 72 || key == 'w') { // Up arrow or 'W'
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
			}
			else if (selectedOption == 2) //Exit Game
			{
				exit(0);
				status == 0;
			} 
				
            system("pause");
            system("cls");
        }
    }
}
