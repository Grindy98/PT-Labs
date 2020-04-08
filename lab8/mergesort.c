#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void merge_sort(int* arr, int n)
{
  if(arr == NULL)
  {
    printf("Given array pointer invalid\n");
    exit(1);
  }
  if(n <= 1)
  {
    // Base case, for sure sorted (single element or no elements)
    return;
  }
  int n1 = n / 2;
  int *arr1 = arr;
  int n2 = n - n1;
  int *arr2 = arr + n1;

  // Sort the 2 split arrays
  merge_sort(arr1, n1);
  merge_sort(arr2, n2);

  // Merge the 2 arrays into a 3rd temp storage
  int final_size = 0;
  int* final = malloc(n * sizeof(int));
  if(final == NULL)
  {
    printf("Not enough memory!\n");
    exit(1);
  }

  int i = 0, j = 0;
  while(1)
  {
    if(arr1[i] <= arr2[j])
    {
      // If they are equal, take the item from arr1 first
      final[final_size++] = arr1[i++];
    }else
    {
      final[final_size++] = arr2[j++];
    }

    // If one of the ends was reached, fill the final array
    // with the other array's elements
    if(i == n1)
    {
      for(; j < n2; j++)
      {
        final[final_size++] = arr2[j];
      }
      break;
    }
    if(j == n2)
    {
      for(; i < n1; i++)
      {
        final[final_size++] = arr1[i];
      }
      break;
    }
  }

  // Putting the result back into the original array
  memcpy(arr, final, sizeof(int) * n);
  free(final);
}


int main()
{
  srand(time(NULL));
  printf("Insert number of elems to sort:");
  int n = 0;
  while(1)
  {
    scanf("%d", &n);
    if(n > 0)
    {
      break;
    }
    printf("\nPlease insert a positive number:");
  }
  int* arr = malloc(n * sizeof(int));
  for(int i = 0; i < n; i++)
  {
    arr[i] = rand() % 10000;
  }
  merge_sort(arr, n);
  for(int i = 0; i < n; i++)
  {
    printf("#%d: %d\n", i, arr[i]);
  }
  return 0;
}
