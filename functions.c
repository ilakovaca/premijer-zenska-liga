#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"

void createTeam() {
    FILE *fp = fopen("teams.dat", "ab");
    if (!fp) {
        perror("Greška pri otvaranju datoteke");
        return;
    }

    Team t;
    printf("Unesi ime tima: ");
    scanf(" %49[^\n]", t.teamName);
    t.gamesPlayed = t.wins = t.losses = t.pointsScored = t.pointsReceived = 0;

    fwrite(&t, sizeof(Team), 1, fp);
    fclose(fp);
}

void readTeams() {
    FILE *fp = fopen("teams.dat", "rb");
    if (!fp) {
        perror("Greška pri otvaranju datoteke");
        return;
    }

    Team t;
    printf("\n--- POPIS TIMOVA ---\n");
    while (fread(&t, sizeof(Team), 1, fp)) {
        printf("Tim: %s, Utakmice: %d, Pobjede: %d, Porazi: %d\n",
               t.teamName, t.gamesPlayed, t.wins, t.losses);
    }
    fclose(fp);
}

void updateTeam() {
    // Placeholder za ažuriranje tima
    printf("Funkcija za ažuriranje nije još implementirana.\n");
}

void insertMatch() {
    // Placeholder za unos utakmice
    printf("Funkcija za unos utakmice nije još implementirana.\n");
}

void deleteTeam() {
    // Placeholder za brisanje tima
    printf("Funkcija za brisanje nije još implementirana.\n");
}

int compareTeams(const void *a, const void *b) {
    return ((Team *)b)->wins - ((Team *)a)->wins;
}

void sortTeams() {
    FILE *fp = fopen("teams.dat", "rb");
    if (!fp) {
        perror("Ne mogu otvoriti datoteku");
        return;
    }

    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    rewind(fp);

    int count = size / sizeof(Team);
    Team *teams = malloc(size);
    if (!teams) {
        perror("Memorija");
        fclose(fp);
        return;
    }

    fread(teams, sizeof(Team), count, fp);
    fclose(fp);

    qsort(teams, count, sizeof(Team), compareTeams);

    printf("\n--- SORTIRANI TIMOVI ---\n");
    for (int i = 0; i < count; i++) {
        printf("%s - Pobjede: %d\n", teams[i].teamName, teams[i].wins);
    }

    free(teams);
}