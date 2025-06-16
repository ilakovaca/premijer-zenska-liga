#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "player.h"
#include "types.h"

void addPlayer() {
    FILE* fp = fopen("players.dat", "ab");
    if (!fp) {
        perror("Greska pri otvaranju datoteke za igrace");
        return;
    }

    Player p;

    printf("Unesite ID tima: ");
    if (scanf("%d", &p.teamId) != 1) {
        printf("Neispravan unos ID-a tima!\n");
        fclose(fp);
        return;
    }
    printf("Unesite broj dresa: ");
    if (scanf("%d", &p.jerseyNumber) != 1) {
        printf("Neispravan unos broja dresa!\n");
        fclose(fp);
        return;
    }
    printf("Unesite prezime i inicijal imena igraca: ");
    if (scanf(" %49[^\n]", p.playerName) != 1) {
        printf("Neispravan unos imena igraca!\n");
        fclose(fp);
        return;
    }
    printf("Unesite godine: ");
    if (scanf("%d", &p.age) != 1) {
        printf("Neispravan unos godina!\n");
        fclose(fp);
        return;
    }
    printf("Unesite visinu (u cm): ");
    if (scanf("%f", &p.height) != 1) {
        printf("Neispravan unos visine!\n");
        fclose(fp);
        return;
    }
    printf("Unesite prosjecne poene po utakmici: ");
    if (scanf("%f", &p.pointsPerGame) != 1) {
        printf("Neispravan unos poena!\n");
        fclose(fp);
        return;
    }

    fwrite(&p, sizeof(Player), 1, fp);
    fclose(fp);

    printf("Igrac uspjesno dodan.\n");
}

void listPlayersByTeam(int teamId) {
    FILE* fp = fopen("players.dat", "rb");
    if (!fp) {
        perror("Greska pri otvaranju datoteke za citanje igraca");
        return;
    }

    Player p;
    printf("\n--- ROSTER TIMA %d ---\n", teamId);
    int found = 0;

    while (fread(&p, sizeof(Player), 1, fp)) {
        if (p.teamId == teamId) {
            printf("Broj dresa: %d, Ime: %s, Godine: %d, Visina: %.1f, Poeni/ut: %.2f\n",
                p.jerseyNumber, p.playerName, p.age, p.height, p.pointsPerGame);
            found = 1;
        }
    }

    if (!found) {
        printf("Nema igraca za ovaj tim.\n");
    }

    fclose(fp);
}

void listAllPlayers() {
    FILE* fp = fopen("players.dat", "rb");
    if (!fp) {
        perror("Greska pri otvaranju datoteke za citanje igraca");
        return;
    }

    FILE* fpTeams = fopen("teams.dat", "rb");
    if (!fpTeams) {
        perror("Greska pri otvaranju datoteke za timove");
        fclose(fp);
        return;
    }

    fseek(fpTeams, 0, SEEK_END);
    long teamFileSize = ftell(fpTeams);
    rewind(fpTeams);

    int teamCount = teamFileSize / sizeof(Team);
    Team* teams = malloc(teamFileSize);
    if (!teams) {
        perror("Greska pri alokaciji memorije za timove");
        fclose(fp);
        fclose(fpTeams);
        return;
    }

    fread(teams, sizeof(Team), teamCount, fpTeams);
    fclose(fpTeams);

    Player p;
    printf("\n--- SVI IGRACI ---\n");
    ispisiIgraceRekurzivno(fp);
    

    fclose(fp);
	free(teams);
    teams = NULL;
}
void ispisiIgraceRekurzivno(FILE* fp) {
    Player p;
    if (fread(&p, sizeof(Player), 1, fp) == 1) {
        printf("Ime: %s  | Broj: %d | PPG: %.2f | Tim ID: %d\n",
            p.playerName, p.jerseyNumber, p.pointsPerGame, p.teamId);
        ispisiIgraceRekurzivno(fp);
	}
}