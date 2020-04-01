#include <stdio.h>
#include <stdlib.h>

int compare(const void* a, const void* b)
{
  return *(int*)a - *(int*)b;
}

int main()
{
  // List of items and the maximum amount stored
  int item_size[] = {1, 1, 1, 1, 6, 3, 2, 9, 2, 1, 5, 4};
  int n = sizeof(item_size)/sizeof(item_size[0]);
  int max_amount = 25;

  qsort(item_size, n, sizeof(int), compare);

  for(int i = 0; i < n; i++)
  {
    if(item_size[i] > max_amount)
    {
      // The next element would go over the limit
      printf("A maximum of %d items fit.\n", i);
      return 0;
    }else
    {
      max_amount -= item_size[i];
    }
  }
  // All elements fit
  printf("All %d items fit.\n", n);
  return 0;
}
