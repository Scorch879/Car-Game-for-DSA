#ifndef FILEHANDLING_H
#define FILEHANDLING_H

#include "game.h"

//Score structure
typedef struct {
	char name[50];
	int score;
}EntryScore;

extern char* filename;

void saveHighScore(const char* filename, EntryScore newEntry);
void displayHighScores(const char* filename);
EntryScore*  loadHighScores(const char* filename, int* count);
int loadHighestScore(const char* filename);

#endif
