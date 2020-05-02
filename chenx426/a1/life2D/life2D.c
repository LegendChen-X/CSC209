#include <stdio.h>
#include <stdlib.h>

#include "life2D_helpers.h"


int main(int argc, char **argv) {

    if (argc != 4) {
        fprintf(stderr, "Usage: life2D rows cols states\n");
        return 1;
    }
    int row = strtol(argv[1],NULL,10);
    int cloumn = strtol(argv[2],NULL,10);

    int table[row * cloumn];
    int ptr = 0;
    int element;

    while (fscanf(stdin, "%d", &element) == 1) table[ptr++] = element;

    for(int i=0;i<strtol(argv[3],NULL,10);++i)
    {
      print_state(table,row,cloumn);
      update_state(table,row,cloumn);
    }
    return 0;
}
