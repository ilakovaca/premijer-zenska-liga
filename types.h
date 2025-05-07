#ifndef TYPES_H
#define TYPES_H

typedef struct {
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

#endif