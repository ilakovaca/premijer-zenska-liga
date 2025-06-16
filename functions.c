#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "functions.h"

int brojTimova = 0;

static int compareByName(const void* a, const void* b) {
    const Team* teamA = (const Team*)a;
    const Team* teamB = (const Team*)b;
    return strcmp(teamA->teamName, teamB->teamName);
}

static int compareByPoints(const void* a, const void* b) {
    int bodoviA = izracunajBodove((const Team*)a);
    int bodoviB = izracunajBodove((const Team*)b);
    return bodoviB - bodoviA;  
}

void createTeam() {
    FILE* fp = fopen("teams.dat", "rb+");
    if (!fp) {
        fp = fopen("teams.dat", "wb+");
        if (!fp) {
            perror("Greska pri stvaranju datoteke");
            return;
        }
    }

    Team t;
    int maxId = 0;
    Team temp;

    while (fread(&temp, sizeof(Team), 1, fp)) {
        if (temp.id > maxId)
            maxId = temp.id;
    }

    t.id = maxId + 1;
    brojTimova++;

    printf("Unesi ime tima: ");
	if (scanf(" %49[^\n]", t.teamName) != 1) {
		fprintf(stderr, "Greska pri unosu imena tima.\n");
		fclose(fp);
		return;
	}
    t.gamesPlayed = t.wins = t.losses = t.pointsScored = t.pointsReceived = 0;

    fseek(fp, 0, SEEK_END);
    fwrite(&t, sizeof(Team), 1, fp);
    fclose(fp);

    printf("Tim '%s' dodan s ID-em %d.\n", t.teamName, t.id);
}
   
void readTeams() {
    FILE* fp = fopen("teams.dat", "rb");
    if (!fp) {
        perror("Greska pri otvaranju datoteke");
        return;
    }

    Team t;
    printf("\n--- POPIS TIMOVA ---\n");
    while (fread(&t, sizeof(Team), 1, fp)) {
        printf("ID: %d, Tim: %s, Utakmice: %d, Pobjede: %d, Porazi: %d\n",
            t.id, t.teamName, t.gamesPlayed, t.wins, t.losses);
    }
    fclose(fp);
}

void updateTeam() {
    FILE* file = fopen("teams.dat", "rb");
    if (!file) {
        perror("Greska pri otvaranju datoteke");
        return;
    }

    FILE* tempFile = fopen("temp.dat", "wb");
    if (!tempFile) {
        perror("Greska pri stvaranju privremene datoteke");
        fclose(file);
        return;
    }

    int idToUpdate;
    printf("Unesite ID tima za azuriranje: ");
	if (scanf("%d", &idToUpdate) != 1) {
		fprintf(stderr, "Greska pri unosu ID-a.\n");
		fclose(file);
		fclose(tempFile);
		return;
	}

    Team team;
    int found = 0;

    while (fread(&team, sizeof(Team), 1, file)) {
        if (team.id == idToUpdate) {
            printf("Unesite novo ime tima: ");
			if (scanf(" %49[^\n]", team.teamName) != 1) {
				fprintf(stderr, "Greska pri unosu imena tima.\n");
				fclose(file);
				fclose(tempFile);
				return;
			}
            found = 1;
        }
        fwrite(&team, sizeof(Team), 1, tempFile);
    }

    fclose(file);
    fclose(tempFile);

    if (found) {
        remove("teams.dat");
        rename("temp.dat", "teams.dat");
        printf("Tim azuriran.\n");
    }
    else {
        remove("temp.dat");
        printf("Tim s ID-om %d nije pronaðen.\n", idToUpdate);
    }
}


void insertMatch() {
    Match match;
    FILE* matchFile = fopen("matches.dat", "ab");
    FILE* teamFile = fopen("teams.dat", "rb+");

    if (!matchFile || !teamFile) {
        perror("Greška pri otvaranju datoteka");
        return;
    }

    printf("Unesi domaci tim: ");
	if (scanf(" %49[^\n]", match.homeTeam) != 1) {
		fprintf(stderr, "Greška pri unosu imena domaæeg tima.\n");
		fclose(matchFile);
		fclose(teamFile);
		return;
	}
    printf("Unesi gostujuci tim: ");
	if (scanf(" %49[^\n]", match.awayTeam) != 1) {
		fprintf(stderr, "Greška pri unosu imena gostujuæeg tima.\n");
		fclose(matchFile);
		fclose(teamFile);
		return;
	}
    printf("Poeni domaceg tima: ");
	if (scanf("%d", &match.homePoints) != 1) {
		fprintf(stderr, "Greška pri unosu poena domaæeg tima.\n");
		fclose(matchFile);
		fclose(teamFile);
		return;
	}
    printf("Poeni gostujuceg tima: ");
	if (scanf("%d", &match.awayPoints) != 1) {
		fprintf(stderr, "Greška pri unosu poena gostujuæeg tima.\n");
		fclose(matchFile);
		fclose(teamFile);
		return;
	}

    fwrite(&match, sizeof(Match), 1, matchFile);
    fclose(matchFile);

    Team team;
    int homeUpdated = 0, awayUpdated = 0;

    fseek(teamFile, 0, SEEK_SET);
    while (fread(&team, sizeof(Team), 1, teamFile)) {
        long pos = ftell(teamFile) - sizeof(Team);

        if (strcmp(team.teamName, match.homeTeam) == 0) {
            team.gamesPlayed++;
            team.pointsScored += match.homePoints;
            team.pointsReceived += match.awayPoints;
            if (match.homePoints > match.awayPoints) team.wins++;
            else team.losses++;

            fseek(teamFile, pos, SEEK_SET);
            fwrite(&team, sizeof(Team), 1, teamFile);
            fflush(teamFile);
            homeUpdated = 1;
        }

        if (strcmp(team.teamName, match.awayTeam) == 0) {
            team.gamesPlayed++;
            team.pointsScored += match.awayPoints;
            team.pointsReceived += match.homePoints;
            if (match.awayPoints > match.homePoints) team.wins++;
            else team.losses++;

            fseek(teamFile, pos, SEEK_SET);
            fwrite(&team, sizeof(Team), 1, teamFile);
            fflush(teamFile);
            awayUpdated = 1;
        }

        if (homeUpdated && awayUpdated) break;
    }

    fclose(teamFile);

    if (!homeUpdated || !awayUpdated) {
        printf("Jedan ili oba tima nisu pronadena!\n");
    }
    else {
        printf("Utakmica spremljena i statistike azurirane.\n");
    }
}

void deleteTeam() {
    FILE* file = fopen("teams.dat", "rb");
    if (!file) {
        perror("Greska pri otvaranju datoteke");
        return;
    }

    FILE* tempFile = fopen("temp.dat", "wb");
    if (!tempFile) {
        perror("Greska pri stvaranju privremene datoteke");
        fclose(file);
        return;
    }

    int idToDelete;
    printf("Unesite ID tima koji zelite obrisati: ");
	if (scanf("%d", &idToDelete) != 1) {
		fprintf(stderr, "Greska pri unosu ID-a.\n");
		fclose(file);
		fclose(tempFile);
		return;
	}

    Team team;
    int found = 0;

    while (fread(&team, sizeof(Team), 1, file)) {
        if (team.id != idToDelete) {
            fwrite(&team, sizeof(Team), 1, tempFile);
        }
        else {
            found = 1;
        }
    }

    fclose(file);
    fclose(tempFile);

    if (!found) {
        printf("Tim s ID-om %d nije pronaden.\n", idToDelete);
        remove("temp.dat");
        return;
    }

    remove("teams.dat");
    rename("temp.dat", "teams.dat");

    printf("Tim uspjesno obrisan.\n");
}


static int compareTeams(const void* a, const void* b) {
    return ((Team*)b)->wins - ((Team*)a)->wins;
}

void sortTeams() {
    FILE* fp = fopen("teams.dat", "rb");
    if (!fp) {
        perror("Ne mogu otvoriti datoteku");
        return;
    }

    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    rewind(fp);

    int count = size / sizeof(Team);
    Team* teams = malloc(size);
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
	teams = NULL;
}

void searchTeamByName() {
  FILE* fp = fopen("teams.dat", "rb");
    if (!fp) {
        perror("Greška pri otvaranju datoteke");
        return;
    }

    fseek(fp, 0, SEEK_END);
    long fileSize = ftell(fp);
    rewind(fp);

    int count = fileSize / sizeof(Team);
    if (count == 0) {
        printf("Nema timova u datoteci.\n");
        fclose(fp);
        return;
    }

    Team* teams = malloc(count * sizeof(Team));
    if (!teams) {
        perror("Neuspješna alokacija memorije");
        fclose(fp);
        return;
    }

    fread(teams, sizeof(Team), count, fp);
    fclose(fp);

    qsort(teams, count, sizeof(Team), compareByName);

    char nameToFind[50];
    printf("Unesite ime tima za pretragu: ");
    if (scanf(" %49[^\n]", nameToFind) != 1) {
		printf("Greška pri unosu imena tima.\n");
		free(teams);
		return;
	}

    Team key;
    strcpy(key.teamName, nameToFind);

    Team* found = bsearch(&key, teams, count, sizeof(Team), compareByName);

    if (found != NULL) {
        printf("Tim pronaðen: %s\n", found->teamName);
        printf("Utakmice: %d, Pobjede: %d, Porazi: %d\n",
               found->gamesPlayed, found->wins, found->losses);
    } else {
        printf("Tim nije pronaðen.\n");
    }

    free(teams);
	teams = NULL;
}
void prikaziTablicuPoretka() {
    FILE* fp = fopen("teams.dat", "rb");
    if (!fp) {
        perror("Greska pri otvaranju datoteke s timovima");
        return;
    }

    fseek(fp, 0, SEEK_END);
    long velicina = ftell(fp);
    rewind(fp);

    int brojTimova = velicina / sizeof(Team);
    if (brojTimova == 0) {
        printf("Nema registriranih timova.\n");
        fclose(fp);
        return;
    }

    Team* timovi = malloc(velicina);
    if (!timovi) {
        perror("Greška pri alokaciji memorije");
        fclose(fp);
        return;
    }

    fread(timovi, sizeof(Team), brojTimova, fp);
    fclose(fp);

    qsort(timovi, brojTimova, sizeof(Team), compareByPoints);

    printf("\n--- TABLICA PORETKA ---\n");
    printf("%-5s %-20s %-10s %-10s %-10s %-6s\n", "Rbr", "Tim", "Utakmice", "Pobjede", "Porazi", "Bodovi");
    for (int i = 0; i < brojTimova; i++) {
        int bodovi = izracunajBodove(&timovi[i]);
        printf("%-5d %-20s %-10d %-10d %-10d %-6d\n", i + 1,
            timovi[i].teamName,
            timovi[i].gamesPlayed,
            timovi[i].wins,
            timovi[i].losses,
            bodovi);
    }

    free(timovi);
	timovi = NULL;
}

