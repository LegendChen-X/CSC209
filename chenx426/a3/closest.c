#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <getopt.h>

#include "point.h"
#include "utilities_closest.h"
#include "serial_closest.h"
#include "parallel_closest.h"


void print_usage() {
    fprintf(stderr, "Usage: closest -f filename -d pdepth\n\n");
    fprintf(stderr, "    -d Maximum process tree depth\n");
    fprintf(stderr, "    -f File that contains the input points\n");

    exit(1);
}

int main(int argc, char **argv) {
    int n = -1;
    long pdepth = -1;
    char *filename = NULL;
    int pcount = 0;
    
    if(argc!=5) print_usage();
    
    opterr=0;
    int opt;
    int flag_d = 0;
    int flag_f = 0;
    while((opt=getopt(argc,argv,"f:d:"))!=-1)
    {
        switch(opt)
        {
            case 'd':
                pdepth=strtol(optarg,NULL,10);
                flag_d=1;
                break;
            case 'f':
                filename=optarg;
                flag_f=1;
                break;
            case '?':
                print_usage();
                break;
            case ':':
                print_usage();
                break;
        }
    }
    if((flag_d+flag_f)!=2) print_usage();


    n = total_points(filename);
    struct Point points_arr[n];
    read_points(filename, points_arr);

    // Sort the points
    qsort(points_arr, n, sizeof(struct Point), compare_x);

    // Calculate the result using the parallel algorithm.
    double result_p = closest_parallel(points_arr, n, pdepth, &pcount);
    printf("The smallest distance: is %.2f (total worker processes: %d)\n", result_p, pcount);
    exit(0);
}
