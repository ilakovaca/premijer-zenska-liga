#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "functions.h"
#include "player.h"

typedef enum {
    DODAJ_TIM = 1,
    PRIKAZ_TIMOVA,
    BRISI_TIM,
    UNESI_UTAKMICU,
    SORTIRAJ_TIMOVE,
    PRETRAZI_TIM,
    AZURIRAJ_TIM,
    DODAJ_IGRACA,
    ROSTER_TIMA,
    SVI_IGRACI,
    PRIKAZI_TABLICU,
    IZLAZ
} IzbornikOpcije;

int main() {
    int izbor;

    do {
        printf("\n--- IZBORNIK ---\n");
        printf("1. Dodaj tim\n");
        printf("2. Prikazi timove\n");
        printf("3. Obrisi tim\n");
        printf("4. Unesi utakmicu\n");
        printf("5. Sortiraj timove po pobjedama\n");
        printf("6. Pretrazi tim po imenu\n");
        printf("7. Azuriraj tim\n");
        printf("8. Dodaj igraca\n");
        printf("9. Prikazi roster tima\n");
		printf("10. Prikazi sve igrace\n");
        printf("11. Prikazi tablicu poretka\n");
        printf("12. Izlaz\n");
        printf("Odabir: ");
        if (scanf("%d", &izbor) != 1) {
            printf("Neispravan unos opcije!\n");
			while (getchar() != '\n'); 
            izbor = 0;
        }

        switch (izbor) {
        case DODAJ_TIM:
            createTeam();
            break;
        case PRIKAZ_TIMOVA:
            readTeams();
            printf("Ukupan broj timova: %d\n", brojTimova);
            break;
        case BRISI_TIM:
            deleteTeam();
            break;
        case UNESI_UTAKMICU:
            insertMatch();
            break;
        case SORTIRAJ_TIMOVE:
            sortTeams();
            break;
        case PRETRAZI_TIM:
            searchTeamByName();
            break;
        case AZURIRAJ_TIM:
            updateTeam();
            break;
		case DODAJ_IGRACA:
			addPlayer();
            break;
		case ROSTER_TIMA:
        {
            int teamId;
            printf("Unesite ID tima: ");
            if (scanf("%d", &teamId) != 1) {
                printf("Neispravan unos ID-a!\n");
                break;
            }
            listPlayersByTeam(teamId);
		}
        case PRIKAZI_TABLICU:
            prikaziTablicuPoretka();
            break;
		case SVI_IGRACI:
			listAllPlayers();
			break;
        case IZLAZ:
            printf("Izlaz iz programa.\n");
            break;
        default:
            printf("Nevazeci izbor, pokusajte ponovno.\n");
        }
    } while (izbor != IZLAZ);

    return 0;
}