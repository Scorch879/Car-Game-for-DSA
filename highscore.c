#include "highscore.h"

void saveHighScore() {
	
	FILE *file = fopen("highscore.txt", "r");
	
	//If the file does not exist
	if (!file) {
		file = fopen("highscore.txt", "w");
		if (!file) {	
            printf("\nError: Unable to create highscore file.\n\n");
            return;
        }
        	fprintf(file, "0"); // Initialize with 0 if no previous high score exists
		fclose(file);
	}
		
	else {
		fclose(file);
	}
	
    if (score > highScore) {
        FILE* file = fopen("highscore.txt", "w");
        if (file) {
            fprintf(file, "%d", score);
            fclose(file);
        }
        else {
        	printf("\nError: Unable to create highscore file.\n\n");
		}
    }
}

void loadHighScore() {
     FILE* file = fopen("highscore.txt", "r");
    if (file) {
        if (fscanf(file, "%d", &highScore) != 1) {
            highScore = 0; // Default to 0 if file is empty or invalid
        }
        fclose(file);
    } else {
        highScore = 0; // Default to 0 if file does not exist
    }
}
