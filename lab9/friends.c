#include <stdio.h>
#include <stdlib.h>
#define N_FRIENDS 6
int is_friend(int a, int b, int friendship_mat[N_FRIENDS][N_FRIENDS])
{
  // Both have to be friends to stand next to each other
  return friendship_mat[a - 1][b - 1] && friendship_mat[b - 1][a - 1];
}

void print_sol(int arrangement[])
{
  for(int i = 0; i < N_FRIENDS; i++)
  {
    printf("%d, ", arrangement[i]);
  }
  printf("\n");
}

int is_valid(int fr, int arrangement[], const int n, int friendship_mat[N_FRIENDS][N_FRIENDS])
{
  // Check for duplicates
  for(int i = 0; i < n; i++)
  {
    if(arrangement[i] == fr)
    {
      return 0;
    }
  }
  // Check to see if they are friends with the person before
  if(is_friend(arrangement[n - 1], fr, friendship_mat))
  {
    return 1;
  }
  return 0;
}

void get_valid_arrangements_recurs(int arrangement[],
   int step, int friendship_mat[N_FRIENDS][N_FRIENDS])
{

  if(step == N_FRIENDS)
  {
    // Array filled, only check if last element positioned properly
    if(is_friend(arrangement[step - 1], arrangement[0], friendship_mat))
    {
      print_sol(arrangement);
      return;
    }
  }
  // Check through all friends (except the first)
  for(int i = 2; i <= N_FRIENDS; i++)
  {
    if(is_valid(i, arrangement, step, friendship_mat))
    {
      // Add each valid friend in the next place
      arrangement[step] = i;
      get_valid_arrangements_recurs(arrangement, step + 1, friendship_mat);
    }
  }
}

void get_valid_arrangements(int arrangement[], int friendship_mat[N_FRIENDS][N_FRIENDS])
{
  // Put the first friend at the beginning of the array to avoid identical solutions
  // for a round table setup ( there will be n_friends solutions for each valid permutation)
  arrangement[0] = 1;
  get_valid_arrangements_recurs(arrangement, 1, friendship_mat);
}

int main()
{
  int friendship_mat[N_FRIENDS][N_FRIENDS] =
  {
    {0, 1, 1, 1, 1, 0,},
    {1, 0, 1, 0, 1, 0,},
    {1, 1, 0, 1, 1, 1,},
    {1, 0, 1, 0, 0, 0,},
    {1, 1, 1, 0, 0, 1,},
    {0, 0, 1, 0, 1, 0,},
  };
  // Solution array with exactly n_friends values
  int arrangement[N_FRIENDS];
  printf("The valid arrangements are:\n");
  get_valid_arrangements(arrangement, friendship_mat);
  return 0;
}
