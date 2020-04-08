#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int get_common_prefix_size(char* a, char* b)
{
  int i = 0;
  while(a[i] != '\0' && b[i] != '\0')
  {
    if(a[i] != b[i])
    {
      break;
    }
    i++;
  }
  return i;
}

int prefix(char* arr[], int n)
{
  if( n <= 0)
  {
    printf("Function cannot take an empty array as param!\n");
    exit(1);
  }else if( n == 1)
  {
    // Base case -- prefix is whole array
    return strlen(arr[0]);
  }
  int n1 = n / 2;
  int n2 = n - n1;
  char** arr1 = arr;
  char** arr2 = arr + n1;

  // Divide arr into 2 subsets
  int i1 = prefix(arr1, n1);
  int i2 = prefix(arr2, n2);

  // Construct the prefixes of both blocks and return the final size
  // The first string of each array is enough to reconstruct the prefix
  char* pref1 = strndup(arr1[0], i1);
  char* pref2 = strndup(arr2[0], i2);
  if(pref1 == NULL || pref2 == NULL)
  {
    printf("Not enough memory!\n");
    exit(1);
  }
  int final_size = get_common_prefix_size(pref1, pref2);
  free(pref1);
  free(pref2);
  return final_size;
}

int main()
{
  char* arr[] = {"Alice", "Alicia", "Almanac", "Altitude", "Altimeter"};
  int n = sizeof(arr) / sizeof(arr[0]);
  int i = prefix(arr, n);
  char* pref = strndup(arr[0], i);
  if(pref == NULL)
  {
    printf("Not enough memory!\n");
    exit(1);
  }
  printf("The prefix is %s\n", pref);
  return 0;
}
