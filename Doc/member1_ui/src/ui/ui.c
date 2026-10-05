#include <stdio.h>

int main(void)
{
    printf("========================================\n");
    printf("          PBL PROCESS SIMULATOR\n");
    printf("========================================\n\n");

    printf("Status: READY\n\n");

    printf("1. Load Program\n");
    printf("2. Run Program\n");
    printf("3. Stop Program\n");
    printf("4. Reset Simulator\n");
    printf("5. Exit\n");

    printf("\nEnter choice: ");
    int choice;
    scanf("%d", &choice);
    printf("\nYou selected: %d\n", choice);

    return 0;
}