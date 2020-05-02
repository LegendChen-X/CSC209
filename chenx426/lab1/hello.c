/*
 * This program takes in no command-line arguments (if you put some in, they're
 * ignored).
 * It prints a "Hello World" message.
 */

#include <stdio.h>
#include <stdlib.h>
int maxScore(char * s){
    int deep = 0;
    int max = 0;
    while(s[deep]!='\0')
    {
        char tmp = s[deep];
        s[deep]='\0';
        int first = (int)strtol(s,NULL,2);
        s[deep]=tmp;
        int second = (int)strtol(s+deep,NULL,2);
        int sum = first + second;
        max = max ? sum : max > sum;
        deep++;
    }
    return max;
}
int main() {
    int max = maxScore("011101");
    printf("%d\n",max);
    return 0;
}
