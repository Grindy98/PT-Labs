#include <stdio.h>

long mypow(long base, unsigned e)
{
  if(e == 0)
  {
    return 1;
  }
  long temp = mypow(base, e / 2);
  if(e % 2 == 0)
  {
    return temp * temp;
  }
  return base * temp * temp;
}

int main()
{
  printf("2^10 = %ld\n", mypow(2, 10));
  printf("3^7 = %ld\n", mypow(3, 7));
  return 0;
}
