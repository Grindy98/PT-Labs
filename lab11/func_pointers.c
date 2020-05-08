#include <stdio.h>
#include <stdarg.h>

typedef struct OBJ
{
  int (*func_sum)(int n, ...);
  int (*func_prod)(int n, ...);
  float (*func_avg)(int n, ...);
  void (*func_show)(int n, ...);
} OBJ;

int sum(int n, ...)
{
  va_list valist;
  va_start(valist, n);
  int s = 0;
  for(int i = 0; i < n; i++)
  {
    s += va_arg(valist, int);
  }
  return s;
}

int prod(int n, ...)
{
  va_list valist;
  va_start(valist, n);
  int p = 1;
  for(int i = 0; i < n; i++)
  {
    p *= va_arg(valist, int);
  }
  return p;
}

float avg(int n, ...)
{
  va_list valist;
  va_start(valist, n);
  float s = 0;
  for(int i = 0; i < n; i++)
  {
    s += va_arg(valist, int);
  }
  return s/n;
}

void show(int n, ...)
{
  va_list valist;
  va_start(valist, n);
  for(int i = 0; i < n; i++)
  {
    printf("%d ",va_arg(valist, int));
  }
  printf("\n");
}

int main()
{
  OBJ o = {sum, prod, avg, show};
  printf("Sum:%d\n", o.func_sum(4, 1, 4, 2, 3));
  printf("Product:%d\n", o.func_prod(4, 1, 4, 2, 3));
  printf("Average:%f\n", o.func_avg(4, 1, 4, 2, 3));
  printf("Show:");
  o.func_show(4, 1, 4, 2, 3);
  return 0;
}
