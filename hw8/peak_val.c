#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define SIZE 100

int find_max(int arr[SIZE][SIZE], int i_left, int i_right)
{
  // Act like arr is an 1D array with arr[i%SIZE][i/SIZE] an element
  if(i_left == i_right)
  {
    // Base case, return the element
    return arr[i_left%SIZE][i_left/SIZE];
  }else if(i_left == i_right - 1)
  {
    // Also a base case (for efficiency), avoid another 2 unnecesary fct calls
    return arr[i_left%SIZE][i_left/SIZE] > arr[i_right%SIZE][i_right/SIZE] ?
    arr[i_left%SIZE][i_left/SIZE] : arr[i_right%SIZE][i_right/SIZE];
  }
  // Else check right and left of the middle point
  int m = (i_left + i_right) / 2;
  int m1 = find_max(arr, i_left, m);
  int m2 = find_max(arr, m + 1, i_right);
  return m1 > m2 ? m1 : m2;
}

int main()
{
  srand(time(NULL));
  int arr[SIZE][SIZE];
  for(int i = 0; i < SIZE; i++)
  {
    for(int j = 0; j < SIZE; j++)
    {
      arr[i][j] = rand() % 100000;
    }
  }
  printf("The max value is %d.\n", find_max(arr, 0, SIZE * SIZE - 1));
  return 0;
}
