#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct PAIR
{
  int i;
  int j;
  int k;
} PAIR;

PAIR ** sols = NULL;
int n_sols = 0;

void check_err(void* a)
{
  if(a == NULL)
  {
    printf("Not enough memory!\n");
    exit(1);
  }
}

int isMazeLine(char* s, int n)
{
  for(int i = 0; i < n; i++)
  {
    if(strchr("#.", s[i]) == NULL)
    {
      return 0;
    }
  }
  return 1;
}

void init_maze(char ****maze, PAIR *maze_dim)
{
  *maze_dim = (PAIR){5, 5, 3};
  char temp_arr[3][5][5] = {
    {
      "#####",
      "#####",
      "#####",
      "#.###",
      "#####",
    },
    {
      "#####",
      "#...#",
      "#...#",
      "#...#",
      "#####",
    },
    {
      "#####",
      "###.#",
      "#####",
      "#####",
      "#####",
    },
  };
  // Dinamically allocate the 3D maze
  *maze = malloc(maze_dim->k * sizeof(char**));
  check_err(*maze);
  for(int k = 0; k < maze_dim->k; k++)
  {
    (*maze)[k] = malloc(maze_dim->i * sizeof(char*));
    check_err((*maze)[k]);
    for(int i = 0; i < maze_dim->i; i++)
    {
      (*maze)[k][i] = strdup(temp_arr[k][i]);
      check_err((*maze)[k][i]);
    }
  }
}

int find_optimal_path(PAIR curr, char*** maze, PAIR m_size, int step)
{
  // Check if start is empty
  if(maze[curr.k][curr.i][curr.j] != '.')
  {
    printf("Function should only be called on empty coords!\n");
    exit(1);
  }
  // If solution is found
  if(curr.i + 1 == m_size.i || curr.i == 0 ||
     curr.j + 1 == m_size.j || curr.j == 0 ||
     curr.k + 1 == m_size.k || curr.k == 0 )
  {
    return step;
  }
  // Mark tile with X and add it to the path
  maze[curr.k][curr.i][curr.j] = 'X';

  int stuck = 1;
  int minimum = -1;
  if(maze[curr.k][curr.i + 1][curr.j] == '.')
  {
    // Top tile
    int ret_val = find_optimal_path((PAIR){curr.i + 1, curr.j, curr.k}, maze, m_size, step + 1);
    if(ret_val != -1)
    {
      stuck = 0;
      if(minimum == -1)
      {
        minimum = ret_val;
      }else if(minimum > ret_val)
      {
        minimum = ret_val;
      }
    }
  }
  if(maze[curr.k][curr.i - 1][curr.j] == '.')
  {
    // Bottom tile
    int ret_val = find_optimal_path((PAIR){curr.i - 1, curr.j, curr.k}, maze, m_size, step + 1);
    if(ret_val != -1)
    {
      stuck = 0;
      if(minimum == -1)
      {
        minimum = ret_val;
      }else if(minimum > ret_val)
      {
        minimum = ret_val;
      }
    }
  }
  if(maze[curr.k][curr.i][curr.j + 1] == '.')
  {
    // Right tile
    int ret_val = find_optimal_path((PAIR){curr.i, curr.j + 1, curr.k}, maze, m_size, step + 1);
    if(ret_val != -1)
    {
      stuck = 0;
      if(minimum == -1)
      {
        minimum = ret_val;
      }else if(minimum > ret_val)
      {
        minimum = ret_val;
      }
    }
  }
  if(maze[curr.k][curr.i][curr.j - 1] == '.')
  {
    // Left tile
    int ret_val = find_optimal_path((PAIR){curr.i, curr.j - 1, curr.k}, maze, m_size, step + 1);
    if(ret_val != -1)
    {
      stuck = 0;
      if(minimum == -1)
      {
        minimum = ret_val;
      }else if(minimum > ret_val)
      {
        minimum = ret_val;
      }
    }
  }
  if(maze[curr.k + 1][curr.i][curr.j] == '.')
  {
    // Above tile
    int ret_val = find_optimal_path((PAIR){curr.i, curr.j, curr.k + 1}, maze, m_size, step + 1);
    if(ret_val != -1)
    {
      stuck = 0;
      if(minimum == -1)
      {
        minimum = ret_val;
      }else if(minimum > ret_val)
      {
        minimum = ret_val;
      }
    }
  }
  if(maze[curr.k - 1][curr.i][curr.j] == '.')
  {
    // Below tile
    int ret_val = find_optimal_path((PAIR){curr.i, curr.j, curr.k - 1}, maze, m_size, step + 1);
    if(ret_val != -1)
    {
      stuck = 0;
      if(minimum == -1)
      {
        minimum = ret_val;
      }else if(minimum > ret_val)
      {
        minimum = ret_val;
      }
    }
  }
  // Delete the mark X
  maze[curr.k][curr.i][curr.j] = '.';
  if(stuck)
  {
    return -1;
  }
  return minimum;
}

void path_print(PAIR curr, char*** maze, PAIR m_size, int step, int minimum)
{
  static PAIR curr_path[10000];
  // Check if start is empty
  if(maze[curr.k][curr.i][curr.j] != '.')
  {
    printf("Function should only be called on empty coords!\n");
    exit(1);
  }
  // Add current path to the solution
  curr_path[step] = curr;
  // If solution is found
  if(curr.i + 1 == m_size.i || curr.i == 0 ||
     curr.j + 1 == m_size.j || curr.j == 0 ||
     curr.k + 1 == m_size.k || curr.k == 0 )
    {
      // Skip over printing
      if(step != minimum)
      {
        return;
      }
      // Print the path
      for(int i = 0; i <= step; i++)
      {
        printf("(%d, %d, %d)->", curr_path[i].i, curr_path[i].j, curr_path[i].k);
      }
      printf("DONE\n");
      return;
    }
    // If minimum smaller than step => return bc the min path can no longer be found
    if(minimum <= step)
    {
      return;
    }
    // Mark tile with X and add it to the path
    maze[curr.k][curr.i][curr.j] = 'X';

    if(maze[curr.k][curr.i + 1][curr.j] == '.')
    {
      // Top tile
      path_print((PAIR){curr.i + 1, curr.j, curr.k}, maze, m_size, step + 1, minimum);
    }
    if(maze[curr.k][curr.i - 1][curr.j] == '.')
    {
      // Bottom tile
      path_print((PAIR){curr.i - 1, curr.j, curr.k}, maze, m_size, step + 1, minimum);
    }
    if(maze[curr.k][curr.i][curr.j + 1] == '.')
    {
      // Right tile
      path_print((PAIR){curr.i, curr.j + 1, curr.k}, maze, m_size, step + 1, minimum);
    }
    if(maze[curr.k][curr.i][curr.j - 1] == '.')
    {
      // Left tile
      path_print((PAIR){curr.i, curr.j - 1, curr.k}, maze, m_size, step + 1, minimum);
    }
    if(maze[curr.k + 1][curr.i][curr.j] == '.')
    {
      // Above tile
      path_print((PAIR){curr.i, curr.j, curr.k + 1}, maze, m_size, step + 1, minimum);
    }
    if(maze[curr.k - 1][curr.i][curr.j] == '.')
    {
      // Below tile
      path_print((PAIR){curr.i, curr.j, curr.k - 1}, maze, m_size, step + 1, minimum);
    }
    // Delete the mark X
    maze[curr.k][curr.i][curr.j] = '.';
  }

int main()
{
  char ***maze = NULL;
  PAIR maze_dim;
  init_maze(&maze, &maze_dim);
  int minimum = find_optimal_path((PAIR){2, 2, 1}, maze, maze_dim, 0);

  path_print((PAIR){2, 2, 1}, maze, maze_dim, 0, minimum);
  for(int k = 0; k < maze_dim.k; k++)
  {
    for(int i = 0; i < maze_dim.i; i++)
    {
      free(maze[k][i]);
    }
    free(maze[k]);
  }
  free(maze);
  return 0;
}
