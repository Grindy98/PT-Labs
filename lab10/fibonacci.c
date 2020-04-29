#include <stdio.h>
#include <time.h>

unsigned long long fib(int n)
{
  if(n == 0)
  {
    return 0;
  }
  if(n == 1)
  {
    return 1;
  }
  unsigned long long temp = fib(n-1);
  return temp + fib(n-2);
}

long long fib2(long long arr[], int n)
{
  if(n == 0)
  {
    return 0;
  }
  if(n == 1)
  {
    return 1;
  }
  if(arr[n - 2] != -1)
  {
    return arr[n - 2];
  }
  unsigned long long temp = fib2(arr, n-1);
  temp += fib2(arr, n-2);
  arr[n - 2] = temp;
  return temp;
}

int main()
{
  // Used to measure the time it takes to execute each chunk of code
  clock_t start;
  printf("Insert the number in the fibonacci sequence:");
  int n = -1;
  scanf("%d", &n);
  while(n <= -1)
  {
    printf("Please enter a positive number:");
    scanf("%d", &n);
  }
  printf("\n");

  start = clock();
  printf("Using only recursion: %llu\n", fib(n));
  printf("Time duration:%fs\n", (float)(-start + clock())/CLOCKS_PER_SEC);

  start = clock();
  long long fib_func_arr[n];
  for(int i = 0; i < n; i++)
  {
    fib_func_arr[i] = -1;
  }
  printf("Using the array in a new function: %lld\n", fib2(fib_func_arr, n));
  printf("Time duration:%fs\n", (float)(-start + clock())/CLOCKS_PER_SEC);

  start = clock();
  unsigned long long fib_arr[n + 2];
  fib_arr[0] = 0;
  fib_arr[1] = 1;
  for(int i = 2; i <= n; i++)
  {
    fib_arr[i] = fib_arr[i - 1] + fib_arr[i - 2];
  }
  printf("Using only the array: %llu\n", fib_arr[n]);
  printf("Time duration:%fs\n", (float)(-start + clock())/CLOCKS_PER_SEC);
}
