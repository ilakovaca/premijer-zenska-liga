#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "player.h"

void viewRoster(const char* teamName) {
    char filename[100];
    snprintf(filename, sizeof(filename), "%s_roster.dat", teamName);

    FILE* fp = fopen(filename, "rb");
    if (!fp) {
        printf("Nema rostera za tim %s.
", teamName);
        return;
    }

    Player p;
    printf("Roster za tim: %s
", teamName);
    while (fread(&p, sizeof(Player), 1, fp)) {
        printf("Ime: %s, Broj: %d, Visina: %.2f m, Poeni/utakmica: %.2f
",
               p.name, p.number, p.height, p.pointsPerGame);
    }
    fclose(fp);
}

void addPlayer(const char* teamName) {
    char filename[100];
    snprintf(filename, sizeof(filename), "%s_roster.dat", teamName);

    FILE* fp = fopen(filename, "ab");
    if (!fp) {
        perror("Greška pri otvaranju roster datoteke");
        return;
    }

    Player p;
    printf("Unesi ime igrača: ");
    scanf(" %49[^
]", p.name);
    printf("Unesi broj dresa: ");
    scanf("%d", &p.number);
    printf("Unesi visinu igrača (u metrima): ");
    scanf("%f", &p.height);
    printf("Unesi prosječne poene po utakmici: ");
    scanf("%f", &p.pointsPerGame);

    fwrite(&p, sizeof(Player), 1, fp);
    fclose(fp);
}