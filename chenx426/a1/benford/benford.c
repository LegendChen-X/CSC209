#include <stdio.h>
#include <stdlib.h>

#include "benford_helpers.h"

/*
 * The only print statement that you may use in your main function is the following:
 * - printf("%ds: %d\n")
 *
 */
int main(int argc, char **argv) {

    if (argc < 2 || argc > 3) {
        fprintf(stderr, "benford position [datafile]\n");
        return 1;
    }
    int element;
    int tally[BASE];
    for(int i=0;i<BASE;++i) tally[i]=0;

    if(argc==3)
    {
      FILE *source = fopen(argv[2],"r");
      if(!source) return 1;
      while(fscanf(source,"%d",&element) == 1) add_to_tally(element,strtol(argv[1],NULL,10),tally);
      if(fclose(source)!=0) return 1;
    }

    else
    {
      while(fscanf(stdin,"%d",&element) != EOF)
      add_to_tally(element,strtol(argv[1],NULL,10),tally);
    }

    for(int i=0;i<BASE;i++) printf("%ds: %d\n",i,tally[i]);
    return 0;
}
