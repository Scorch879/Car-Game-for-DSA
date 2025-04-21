#include "FileHandling.h"

char* filename = "highscores.txt";

// **Function to update or save a user's high score**
void saveHighScore(const char* filename, EntryScore newEntry) 
{
    EntryScore *scores = NULL;
    int count = 0;
    int userFound = 0;

    FILE *file = fopen(filename, "r");
    if (file) 
	{
        while (!feof(file)) 
		{
            scores = realloc(scores, (count + 1) * sizeof(EntryScore)); // Expand dynamically
            if (fscanf(file, "%s %d", scores[count].name, &scores[count].score) == 2) 
			{
                if (strcmp(scores[count].name, newEntry.name) == 0) 
				{
                    if (newEntry.score > scores[count].score) 
					{
                        scores[count].score = newEntry.score; // Update user's high score
                    }
                    userFound = 1;
                }
                count++;
            }
        }
        fclose(file);
    }

    if (!userFound) 
	{ 
        scores = realloc(scores, (count + 1) * sizeof(EntryScore)); // Allocate space for new user
        scores[count] = newEntry;
        count++;
    }

    // Write updated scores back to file
    file = fopen(filename, "w");
    if (file) 
	{
        for (i = 0; i < count; i++) 
		{
            fprintf(file, "%s %d\n", scores[i].name, scores[i].score);
        }
        fclose(file);
    }

    free(scores); // Free allocated memory
}


void displayHighScores(const char* filename) 
{
    int count = 0;
    int startX = WIDTH / 2 + 10; // X-position for right side
    int startY = 4;
    
    EntryScore* scores = loadHighScores(filename, &count);
    if (!scores || count == 0)
    {
    	printf("No high scores available.\n");
    	free(scores);
    	return;
	}
    
    // Sort in descending order
    for (i = 0; i < count - 1; i++) 
	{
        for (j = i + 1; j < count; j++) 
		{
            if (scores[j].score > scores[i].score) 
			{
                EntryScore temp = scores[i];
                scores[i] = scores[j];
                scores[j] = temp;
            }
        }
    }

    // Display top 10 scores
    gotoxy(startX, startY - 2);
    setColor(10);
    printf("===== HIGH SCORES =====");

    int displayCount = (count < 10) ? count : 10;
    for (i = 0; i < displayCount; i++) 
    {
        gotoxy(startX, startY + i);
        printf("%d. %s - %d", i + 1, scores[i].name, scores[i].score);
    }

    setColor(7);
    free(scores);
    
}

EntryScore* loadHighScores(const char* filename, int* count)
{
	EntryScore* scores = NULL;
	*count = 0;
	
	FILE* file = fopen(filename, "r");
	if (file)
	{
		while(!feof(file))
		{
			scores = realloc(scores, (*count + 1) * sizeof(EntryScore));
            if (fscanf(file, "%s %d", scores[*count].name, &scores[*count].score) == 2) 
            {
                (*count)++;
            }
		}
		fclose(file);
	}
	return scores;
}

int loadHighestScore(const char* filename)
{
	EntryScore* scores = NULL;
    int count = 0;
    int highScore = 0;

    FILE* file = fopen(filename, "r");
    if (file) {
        while (!feof(file)) {
            scores = realloc(scores, (count + 1) * sizeof(EntryScore));
            if (fscanf(file, "%s %d", scores[count].name, &scores[count].score) == 2) {
                if (scores[count].score > highScore) {
                    highScore = scores[count].score; // Track the highest score
                }
                count++;
            }
        }
        fclose(file);
    }

    free(scores);
    return highScore; // Return the highest score
}
//void saveHighScore() 
//{
//	
//	FILE *file = fopen("highscore.txt", "r");
//	
//	//If the file does not exist
//	if (!file) {
//		file = fopen("highscore.txt", "a");
//		if (!file) {	
//            printf("\nError: Unable to create highscore file.\n\n");
//            return;
//        }
//        	fprintf(file, "0"); // Initialize with 0 if no previous high score exists
//		fclose(file);
//	}
//		
//	else {
//		fclose(file);
//	}
//	
//    if (score > highScore) 
//	{
//        FILE* file = fopen("highscore.txt", "w");
//        if (file) 
//		{
//            fprintf(file, "%d", score);
//            fclose(file);
//        }
//        else 
//		{
//        	printf("\nError: Unable to create highscore file.\n\n");
//		}
//    }
//}
//

//void loadHighScore() 
//{
//    FILE* file = fopen("highscore.txt", "r");
//    if (file) 
//	{
//        if (fscanf(file, "%d", &highScore) != 1) 
//		{
//            highScore = 0; // Default to 0 if file is empty or invalid
//        }
//        fclose(file);
//    } 
//	else 
//	{
//        highScore = 0; // Default to 0 if file does not exist
//    }
//}
