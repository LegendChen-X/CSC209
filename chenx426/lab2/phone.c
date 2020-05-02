#include <stdio.h>
#include <stdlib.h>
int main() {
  char str[11];
  int num;

  scanf("%s", str);

  scanf("%d", &num);

  if(num==-1) printf("%s\n", str);

  if(0<=num && num<=9) printf("%c\n", str[num]);

  if(num < -1 || num > 9)
  {
    printf("ERROR");
    return 1;
  }

  return 0;
}
