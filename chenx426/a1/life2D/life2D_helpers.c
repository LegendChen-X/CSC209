#include <stdio.h>
#include <stdlib.h>


void print_state(int *board, int num_rows, int num_cols) {
    for (int i = 0; i < num_rows * num_cols; i++) {
        printf("%d", board[i]);
        if (((i + 1) % num_cols) == 0) {
            printf("\n");
        }
    }
    printf("\n");
}

int count_neighbours(int *board, int r, int c, int num_cols)
{
  int count = 0;
  for(int i=-1;i<2;++i)
  {
    for(int j=-1;j<2;++j)
    {
      if(i==0&&j==0) continue;
      if(board[r*num_cols+c+i*num_cols+j]==1) count++;
    }
  }
  return count;
}

void update_state(int *board, int num_rows, int num_cols)
{
    int tmp[num_rows*num_cols];
    for(int i=0;i<num_rows*num_cols;++i) tmp[i]=board[i];

    for(int i=1;i<num_rows-1;++i)
    {
      for(int j=1;j<num_cols-1;++j)
      {
        int count = count_neighbours(tmp,i,j,num_cols);
        if(count<2 || count>3) board[i*num_cols+j] = 0;
        else board[i*num_cols+j] = 1;
      }
    }
}
