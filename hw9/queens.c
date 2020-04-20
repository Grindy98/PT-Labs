#include <stdio.h>
#include <stdlib.h>

int sol_count = 0;
// queens array will represent the column a queen is placed on(given the row)
void print_queen_board(int queens[8])
{
  printf("\n ");
  for(int j = 0; j < 8; j++)
  {
    printf("+---");
  }
  printf("+\n");
  for(int i = 0; i < 8; i++)
  {
    for(int j = 0; j < 8; j++)
    {
      printf(" | ");
      if(queens[i] == j)
      {
        printf("Q");
      }else
      {
        printf(" ");
      }
    }
    printf(" |\n ");
    for(int j = 0; j < 8; j++)
    {
      printf("+---");
    }
    printf("+\n");
  }
  printf("\n");
}

void get_queens(int step)
{
  static int queens[8];
  if(step == 8)
  {
    // We have a solution
    sol_count++;
    print_queen_board(queens);
    return;
  }
  // Check if placement is invalid
  for(int j = 0; j < 8; j++)
  {
    int ok = 1;
    for(int k = 0; k < step; k++)
    {
      if(queens[k] == j)
      {
        // Duplicate col
        ok = 0;
      }else if(j - queens[k] == k - step)
      {
        // Pieces on same primary diagonal y2 - y1 == x2 - x1
        ok = 0;
      }else if(j - queens[k] == step - k)
      {
        // Pieces on same secondary diagonal y2 - y1 == x1 - x2
        ok = 0;
      }
    }
    if(ok)
    {
      // Current column possible
      queens[step] = j;
      get_queens(step + 1);
    }
  }
}

int main()
{
  get_queens(0);
  printf("In total, there are %d solutions.\n", sol_count);
  return 0;
}
