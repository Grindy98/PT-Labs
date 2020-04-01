#include <stdio.h>
#include <stdlib.h>

typedef struct
{
  float value;
  float weight;
} OBJECT;

int compare_val_per_kilo_desc(const void* a1, const void* a2)
{
  OBJECT b1 = *(OBJECT*)a1;
  OBJECT b2 = *(OBJECT*)a2;
  float val = b1.value/b1.weight - b2.value/b2.weight;
  if(val < 0)
  {
    return 1;
  }
  return -1;
}

int main()
{
  OBJECT arr[] = {{4, 6}, {10, 1}, {15, 2}, {7, 0.2}, {7, 0.8}, {9,15}, {2, 0.1}, {3, 0.05}};
  float carry_capacity = 8.5;
  int n = sizeof(arr)/sizeof(arr[0]);

  qsort(arr, n, sizeof(OBJECT), compare_val_per_kilo_desc);

  // Sorted array by value per kilo descending
  float total_value = 0;
  for(int i = 0; i < n; i++)
  {
    // Check if overfilled
    if(arr[i].weight >= carry_capacity)
    {
      // Add a fraction of that object and return result
      float fraction = carry_capacity / arr[i].weight;
      total_value += fraction * arr[i].value;
      printf("%g%% of the last object(value %g, with weight %g) was taken!\n",
      fraction * 100, arr[i].value, arr[i].weight);
      printf("A total of %g value was accumulated.\n", total_value);
      return 0;
    }
    // Add the whole object and subtract available carry cap
    total_value += arr[i].value;
    carry_capacity -= arr[i].weight;
    printf("A whole object(value %g, with weight %g) was taken.\n",
    arr[i].value, arr[i].weight);
  }
  printf("All objects were taken with %g of carrying capacity remaining!\n",
  carry_capacity);
  printf("A total of %g value was accumulated.\n", total_value);
  return 0;
}
