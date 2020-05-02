#include <stdio.h>
#include <stdlib.h>
int main() {
  char str[11];

  scanf("%s", str);

  int num;

  int check = 0;

  while(scanf("%d",&num)!=EOF)
  {
    if(num==-1) printf("%s\n", str);

    if(0<=num && num<=9) printf("%c\n", str[num]);

    if(num < -1 || num > 9)
    {
      printf("ERROR\n");
      check = 1;
    }

  }
  return check;
}
