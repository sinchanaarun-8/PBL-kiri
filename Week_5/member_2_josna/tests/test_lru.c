
#include <stdio.h>
#include "../include/lru.h"

int main(void)
{
    int pages[] = {1, 2, 3, 1, 4, 5};
    int n = sizeof(pages) / sizeof(pages[0]);
    int frames = 3;

    printf("Page Reference String: ");
    for (int i = 0; i < n; i++)
        printf("%d ", pages[i]);

    printf("\nNumber of Frames: %d\n", frames);

    lru_page_replacement(pages, n, frames);

    return 0;
}
