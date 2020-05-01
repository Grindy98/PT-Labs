// Given a list of objects with a weight and a value, and a knapsack with a known capacity,
// Find the maximum value you can obtain by choosing some of the objects so that they do not exceed the capacity
// Any particular object is elementary ( cannot be split down into further pieces )

#include <stdio.h>

typedef struct OBJ
{
  int weight;
  int value;
} OBJ;

int knapsack_rec(const OBJ arr[], const int n, int curr_i, int cap, int intermediate[][n])
{
  if(curr_i >= n)
  {
    // If curr_i is n => end of objects
    return 0;
  }
  if(cap == 0)
  {
    // You can't add objects anymore (for efficiency)
    return 0;
  }
  // If previous computation exists, use it
  if(intermediate[cap][curr_i] != -1)
  {
    return intermediate[cap][curr_i];
  }

  // Store the result when not adding the current element
  int result = knapsack_rec(arr, n, curr_i + 1, cap, intermediate);

  if(cap - arr[curr_i].weight >= 0)
  {
    // Only if current element fits store the result when adding it
    int temp_res = arr[curr_i].value +
      knapsack_rec(arr, n, curr_i + 1, cap - arr[curr_i].weight, intermediate);
    result = result > temp_res ? result : temp_res;
  }

  // Remember the maximum value obtained given the curr_i and the cap
  intermediate[cap][curr_i] = result;
  return result;
}

int knapsack(const OBJ arr[], const int n, int cap)
{
  // Initialize the intermediate array
  int intermediate[cap + 1][n];
  for(int i = 0; i < cap + 1; i++)
  {
    for(int j = 0; j < n; j++)
    {
      intermediate[i][j] = -1;
    }
  }
  return knapsack_rec(arr, n, 0, cap, intermediate);
}

int main()
{
  OBJ arr[] = {{2, 10}, {4, 12}, {2, 5}, {3, 10}, {5, 15}};
  int n = sizeof(arr)/sizeof(arr[0]);
  printf("%d\n", knapsack(arr, n, 8));
  return 0;
}
