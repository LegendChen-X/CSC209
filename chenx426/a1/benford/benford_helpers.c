#include <stdio.h>

#include "benford_helpers.h"

int count_digits(int num) {
    if(num/BASE==0) return 1;
    return 1+count_digits(num/10);
}

int get_ith_from_right(int num, int i) {
    if(i==0) return num%BASE;
    return get_ith_from_right(num/BASE,i-1);
}

int get_ith_from_left(int num, int i) {
    return get_ith_from_right(num,count_digits(num)-1-i);
}

void add_to_tally(int num, int i, int *tally) {
    if(i>count_digits(num)) return;
    tally[get_ith_from_left(num,i)] += 1;
}
