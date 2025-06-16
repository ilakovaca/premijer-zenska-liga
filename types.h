#ifndef TYPES_H
#define TYPES_H
#include <stdio.h>


typedef struct {
    int id;
    char teamName[50];
    int gamesPlayed;
    int wins;
    int losses;
    int pointsScored;
    int pointsReceived;
} Team;

typedef struct {
    char homeTeam[50];
    char awayTeam[50];
    int homePoints;
    int awayPoints;
} Match;

typedef struct {
    int teamId;
    int jerseyNumber;
    char playerName[50];
    int age;
    float height;
    float pointsPerGame;
} Player;

#endif
