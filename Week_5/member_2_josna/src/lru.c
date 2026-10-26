
#include <stdio.h>
#include <stdlib.h>
#include "../include/lru.h"

void lru_page_replacement(int pages[], int n, int frames)
{
    int memory[frames];
    int last_used[frames];
    int hits = 0, faults = 0;
    int time = 0;

    for (int i = 0; i < frames; i++)
    {
        memory[i] = -1;
        last_used[i] = -1;
    }

    printf("\nLRU Page Replacement Algorithm\n");
    printf("Page\tFrames\t\tStatus\n");

    for (int i = 0; i < n; i++)
    {
        int found = 0;

        for (int j = 0; j < frames; j++)
        {
            if (memory[j] == pages[i])
            {
                found = 1;
                last_used[j] = time++;
                hits++;
                break;
            }
        }

        if (!found)
        {
            faults++;

            int position = -1;

            for (int j = 0; j < frames; j++)
            {
                if (memory[j] == -1)
                {
                    position = j;
                    break;
                }
            }

            if (position == -1)
            {
                position = 0;

                for (int j = 1; j < frames; j++)
                {
                    if (last_used[j] < last_used[position])
                        position = j;
                }
            }

            memory[position] = pages[i];
            last_used[position] = time++;
        }

        printf("%d\t", pages[i]);

        for (int j = 0; j < frames; j++)
        {
            if (memory[j] == -1)
                printf("- ");
            else
                printf("%d ", memory[j]);
        }

        printf("\t\t%s\n", found ? "Hit" : "Fault");
    }

    printf("\nTotal Hits: %d\n", hits);
    printf("Total Page Faults: %d\n", faults);
    printf("Hit Ratio: %.2f\n", (float)hits / n);
    printf("Fault Ratio: %.2f\n", (float)faults / n);
}
