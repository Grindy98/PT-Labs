#include <stdio.h>

int max(int a, int b)
{
  return a > b ? a : b;
}

int get_max_subarray(const int* arr, int n)
{
  if(n == 0)
  {
    return 0;
  }
  if(n == 1)
  {
    // Always return at least zero because you can have a empty set as the solution
    return arr[0] > 0 ? arr[0] : 0;
  }
  int n1 = n/2;
  int n2 = n - n1;
  const int* arr1 = arr;
  const int* arr2 = arr + n1;
  int left_max = get_max_subarray(arr1, n1);
  int right_max = get_max_subarray(arr2, n2);
  int overall_max = max(left_max, right_max);

  // Reuse left_max and right_max for the bound sums of the mixed array
  left_max = arr1[n1 - 1];
  right_max = arr2[0];

  //Get the max left sum
  int left_sum_bound = 0;
  for(int i = n1 - 1; i >= 0; i--)
  {
    left_sum_bound += arr1[i];
    if(left_max < left_sum_bound)
    {
      left_max = left_sum_bound;
    }
  }

  // Get the max right sum
  int right_sum_bound = 0;
  for(int i = 0; i < n2; i++)
  {
    right_sum_bound += arr2[i];
    if(right_max < right_sum_bound)
    {
      right_max = right_sum_bound;
    }
  }

  // Return the max val of the overall_max and the middle sums
  return max(overall_max, left_max + right_max);
}

int get_max_subarray_dynamic(int arr[], int n)
{
  int sums[n];
  // Fill first element -- special case;
  sums[0] = arr[0];
  for(int i = 1; i < n; i++)
  {
    if(sums[i - 1] < 0)
    {
      // Ignore the previous sum and only add from here forward
      sums[i] = arr[i];
    }else
    {
      // Add normally
      sums[i] = sums[i - 1] + arr[i];
    }
  }
  // The max value from the array is the sum -- 0 is the minimum ( if we don't choose any elements)
  int max_sum = 0;
  for(int i = 0; i < n; i++)
  {
    if(sums[i] > max_sum)
    {
      max_sum = sums[i];
    }
  }
  return max_sum;
}

int main()
{
  int arr[] = {1, -6, 2, 4, 5, 4, -2, -3, 10, 2, -2, -4, 1};
  int n = sizeof(arr)/sizeof(arr[0]);
  printf("Max subarray via DEI method:%d\n", get_max_subarray(arr, n));
  printf("Max subarray via Dynamic method:%d\n", get_max_subarray_dynamic(arr, n));
}
