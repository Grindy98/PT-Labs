#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct PAIR
{
  int i;
  int j;
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

void read_maze(FILE* fin, char ***maze, int *rows, int *cols)
{
  char** new_arr = NULL;
  int n_lines = 0;
  int line_len = -1;
  int arr_cap = 0;
  while(1)
  {
    // Reallocate memory if too little left
    if(arr_cap < n_lines + 1)
    {
      arr_cap += 10;
      new_arr = realloc(new_arr, arr_cap * sizeof(char*));
      check_err(new_arr);
    }
    // Read the next line
    size_t temp_cap = 0;
    char* temp_line = NULL;
    int temp_len = getline(&temp_line, &temp_cap, fin);
    // Exit condition
    if(temp_len == -1)
    {
      break;
    }
    if(temp_line[temp_len - 1] == '\n')
    {
      // Remove the last char if it is \n
      temp_len--;
      temp_line[temp_len] = '\0';
    }
    // Check the line length, and if it does not match -- invalid file
    if(line_len == -1)
    {
      // Initialize length
      line_len = temp_len;
    }else if(temp_len != line_len)
    {
      printf("File does not encode a rectangular maze!\n");
      exit(1);
    }
    // Check if only # and . characters
    if(isMazeLine(temp_line, temp_len) != 1)
    {
      printf("File does not encode a maze(only # and . characters)!\n");
      exit(1);
    }
    // Store the new line into the array
    new_arr[n_lines++] = temp_line;
  }
  // Cut off all the excess memory
  *maze = realloc(new_arr, n_lines * sizeof(char*));
  *rows = n_lines;
  *cols = line_len;
}

int find_optimal_path(PAIR curr, char** maze, PAIR m_size, int step)
{
  // Check if start is empty
  if(maze[curr.i][curr.j] != '.')
  {
    printf("Function should only be called on empty coords!\n");
    exit(1);
  }
  // If solution is found
  if(curr.i + 1 == m_size.i || curr.i == 0 ||
     curr.j + 1 == m_size.j || curr.j == 0)
  {
    return step;
  }
  // Mark tile with X and add it to the path
  maze[curr.i][curr.j] = 'X';

  int stuck = 1;
  int minimum = -1;
  if(maze[curr.i + 1][curr.j] == '.')
  {
    // Top tile
    int ret_val = find_optimal_path((PAIR){curr.i + 1, curr.j}, maze, m_size, step + 1);
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
  if(maze[curr.i - 1][curr.j] == '.')
  {
    // Bottom tile
    int ret_val = find_optimal_path((PAIR){curr.i - 1, curr.j}, maze, m_size, step + 1);
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
  if(maze[curr.i][curr.j + 1] == '.')
  {
    // Right tile
    int ret_val = find_optimal_path((PAIR){curr.i, curr.j + 1}, maze, m_size, step + 1);
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
  if(maze[curr.i][curr.j - 1] == '.')
  {
    // Left tile
    int ret_val = find_optimal_path((PAIR){curr.i, curr.j - 1}, maze, m_size, step + 1);
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
  maze[curr.i][curr.j] = '.';
  if(stuck)
  {
    return -1;
  }
  return minimum;
}

void path_print(PAIR curr, char** maze, PAIR m_size, int step, int minimum)
{
  static PAIR curr_path[10000];
  // Check if start is empty
  if(maze[curr.i][curr.j] != '.')
  {
    printf("Function should only be called on empty coords!\n");
    exit(1);
  }
  // Add current path to the solution
  curr_path[step] = curr;
  // If solution is found
  if(curr.i + 1 == m_size.i || curr.i == 0 ||
    curr.j + 1 == m_size.j || curr.j == 0)
    {
      // Skip over printing
      if(step != minimum)
      {
        return;
      }
      // Print the path
      for(int i = 0; i <= step; i++)
      {
        printf("(%d, %d)->", curr_path[i].i, curr_path[i].j);
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
    maze[curr.i][curr.j] = 'X';

    if(maze[curr.i + 1][curr.j] == '.')
    {
      // Top tile
      path_print((PAIR){curr.i + 1, curr.j}, maze, m_size, step + 1, minimum);
    }
    if(maze[curr.i - 1][curr.j] == '.')
    {
      // Bottom tile
      path_print((PAIR){curr.i - 1, curr.j}, maze, m_size, step + 1, minimum);
    }
    if(maze[curr.i][curr.j + 1] == '.')
    {
      // Right tile
      path_print((PAIR){curr.i, curr.j + 1}, maze, m_size, step + 1, minimum);
    }
    if(maze[curr.i][curr.j - 1] == '.')
    {
      // Left tile
      path_print((PAIR){curr.i, curr.j - 1}, maze, m_size, step + 1, minimum);
    }
    // Delete the mark X
    maze[curr.i][curr.j] = '.';
  }

void maze_print(PAIR curr, char** maze, PAIR m_size, int step, int minimum)
{
  // Check if start is empty
  if(maze[curr.i][curr.j] != '.')
  {
    printf("Function should only be called on empty coords!\n");
    exit(1);
  }
  // Mark tile with X and add it to the path
  maze[curr.i][curr.j] = 'X';
  // If solution is found
  if(curr.i + 1 == m_size.i || curr.i == 0 ||
    curr.j + 1 == m_size.j || curr.j == 0)
    {
      // Skip over printing
      if(step != minimum)
      {
        maze[curr.i][curr.j] = '.';
        return;
      }
      // Print the matrix
      printf("\n");
      for(int i = 0; i < m_size.i; i++)
      {
        for(int j = 0; j < m_size.j; j++)
        {
          printf("%c", maze[i][j]);
        }
        printf("\n");
      }
      maze[curr.i][curr.j] = '.';
      return;
    }
    // If minimum smaller than step => return bc the min path can no longer be found
    if(minimum <= step)
    {
      maze[curr.i][curr.j] = '.';
      return;
    }

    if(maze[curr.i + 1][curr.j] == '.')
    {
      // Top tile
      maze_print((PAIR){curr.i + 1, curr.j}, maze, m_size, step + 1, minimum);
    }
    if(maze[curr.i - 1][curr.j] == '.')
    {
      // Bottom tile
      maze_print((PAIR){curr.i - 1, curr.j}, maze, m_size, step + 1, minimum);
    }
    if(maze[curr.i][curr.j + 1] == '.')
    {
      // Right tile
      maze_print((PAIR){curr.i, curr.j + 1}, maze, m_size, step + 1, minimum);
    }
    if(maze[curr.i][curr.j - 1] == '.')
    {
      // Left tile
      maze_print((PAIR){curr.i, curr.j - 1}, maze, m_size, step + 1, minimum);
    }
    // Delete the mark X
    maze[curr.i][curr.j] = '.';
}

int main()
{
  char **maze = NULL;
  int n, m;
  FILE* fin = fopen("maze_file", "r");
  if(fin == NULL)
  {
    printf("Cannot open file!\n");
    exit(1);
  }
  read_maze(fin, &maze, &m, &n);
  int minimum = find_optimal_path((PAIR){3, 10}, maze, (PAIR){m, n}, 0);
  printf("The maze path:\n");
  maze_print((PAIR){3, 10}, maze, (PAIR){m, n}, 0, minimum);
  printf("\nThe coordinates path:\n");
  path_print((PAIR){3, 10}, maze, (PAIR){m, n}, 0, minimum);
  for(int i = 0; i < m; i++)
  {
    free(maze[i]);
  }
  free(maze);
  for(int i = 0; i < n_sols; i++)
  {
    free(sols[i]);
  }
  free(sols);
  fclose(fin);
  return 0;
}
