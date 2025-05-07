#ifndef PLAYER_H
#define PLAYER_H

typedef struct {
    char name[50];
    int number;
    float height;
    float pointsPerGame;
} Player;

void viewRoster(const char* teamName);
void addPlayer(const char* teamName);

#endif