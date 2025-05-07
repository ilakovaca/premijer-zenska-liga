#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "types.h"
#include "functions.h"

int main() {
    int choice;

    do {
        printf("\n--- IZBORNIK ---\n");
        printf("1. Dodaj novi tim\n");
        printf("2. Prikazi sve timove\n");
        printf("3. Azuriraj tim\n");
        printf("4. Dodaj utakmicu\n");
        printf("5. Obrisi tim\n");
        printf("6. Sortiraj timove\n");
        printf("0. Izlaz\n");
        printf("Odabir: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1: createTeam(); break;
        case 2: readTeams(); break;
        case 3: updateTeam(); break;
        case 4: insertMatch(); break;
        case 5: deleteTeam(); break;
        case 6: sortTeams(); break;
        case 0: printf("Izlaz...\n"); break;
        default: printf("Neispravan unos!\n"); break;
        }

    } while (choice != 0);

    return 0;
}
