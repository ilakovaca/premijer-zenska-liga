#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "types.h"

extern int brojTimova;

void createTeam();
void readTeams();
void updateTeam();
void insertMatch();
void deleteTeam();
void sortTeams();
void searchTeamByName();
void prikaziTablicuPoretka();

int compareByName(const void* a, const void* b);

inline int izracunajBodove(const Team* t) {
    return t->wins * 2 + t->losses;
}


#endif
