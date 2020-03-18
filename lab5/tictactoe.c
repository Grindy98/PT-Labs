#include "tictactoe.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
  srand(time(NULL));
  // Init grid with spaces only
  for(int i = 0; i < GRID_SIZE; i++)
  {
    for(int j = 0; j < GRID_SIZE; j++)
    {
      grid[i][j] = ' ';
    }
  }

  // Game loop
  int flag;// -1 -> game still going | 0 -> draw |
  // 1 -> X won | 2 -> O won
  while(1)
  {
    // X places
    flag = perform_random_move('X');
    if(flag != -1)
    {
      break;
    }
    // O places
    flag = perform_random_move('O');
    if(flag != -1)
    {
      break;
    }
  }
  // End result
  switch(flag)
  {
    case 0:
      printf("Game ended with a draw!\n");
      break;
    case 1:
      printf("Game ended with player X winning\n");
      break;
    case 2:
      printf("Game ended with player O winning\n");
      break;
    default:
      printf("Undefined result");
      exit(1);
  }
  return 0;
}

int perform_random_move(char player)
{
  IND pos = get_empty_pos(rand() % empty);
  place(player, pos);
  empty--;
  print_grid();
  return check_end(pos);
}

IND get_empty_pos(int index)
{
  // If empty == 0, this should not get called
  if(empty == 0)
  {
    printf("Error, invalid call of get_empty_pos!\n");
    exit(1);
  }
  // Iterate over the grid and when you find a space subtract from index
  for(int i = 0; i < GRID_SIZE; i++)
  {
    for(int j = 0; j < GRID_SIZE; j++)
    {
      if(grid[i][j] == ' ')
      {
        if(index == 0)
        {
          return (IND){i, j};
        }
        index--;
      }
    }
  }
  // This will never happen
  return (IND){0, 0};
}

void place(char player, IND pos)
{
  if(grid[pos.i][pos.j] != ' ')
  {
    printf("Trying to override non-empty grid slot!\n");
    exit(1);
  }
  grid[pos.i][pos.j] = player;
}

void print_grid()
{
  for(int j = 0; j < GRID_SIZE; j++)
  {
    printf("+---");
  }
  printf("+\n");
  for(int i = 0; i < GRID_SIZE; i++)
  {
    for(int j = 0; j < GRID_SIZE; j++)
    {
      printf("| %c ", grid[i][j]);
    }
    printf("|\n");
    //Print a buffer line between
    for(int j = 0; j < GRID_SIZE; j++)
    {
      printf("+---");
    }
    printf("+\n");
  }
  printf("\n");
}

int check_end(IND pos)
{
  char player = grid[pos.i][pos.j];
  // Check column
  int won = 1;
  for(int i = 0; i < GRID_SIZE; i++)
  {
    if(grid[i][pos.j] != player)
    {
      won = 0;
    }
  }
  if(won == 1)
  {
    // Player won on column
    return player == 'X' ? 1 : 2;
  }

  // Check row
  won = 1;
  for(int j = 0; j < GRID_SIZE; j++)
  {
    if(grid[pos.i][j] != player)
    {
      won = 0;
    }
  }
  if(won == 1)
  {
    // Player won on row
    return player == 'X' ? 1 : 2;
  }

  // Check main diagonal
  won = 1;
  for(int i = 0; i < GRID_SIZE; i++)
  {
    if(grid[i][i] != player)
    {
      won = 0;
    }
  }
  if(won == 1)
  {
    // Player won on main diagonal
    return player == 'X' ? 1 : 2;
  }

  // Check other diagonal
  won = 1;
  for(int j = 0; j < GRID_SIZE; j++)
  {
    if(grid[j][GRID_SIZE - j - 1] != player)
    {
      won = 0;
    }
  }
  if(won == 1)
  {
    // Player won on row
    return player == 'X' ? 1 : 2;
  }

  // If no other options, then it is a draw or
  // the game has not ended yet
  if(empty != 0)
  {
    return -1;
  }
  return 0;
}
