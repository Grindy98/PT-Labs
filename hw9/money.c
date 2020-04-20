#include <stdio.h>
#include <stdlib.h>

#define N_COINS 5
int denoms[N_COINS] = {1, 5, 10, 20, 50};

void print_coins(int coins[N_COINS])
{
  for(int i = 0; i < N_COINS; i++)
  {
    printf("%d x coins of val %d.\n", coins[i], denoms[i]);
  }
  printf("\n");
}

void get_coins(int total_val, int coins[N_COINS], int min_coin)
{
  if(total_val < 0)
  {
    // No solution possible
    return;
  }
  if(total_val == 0)
  {
    // Solution found
    print_coins(coins);
    return;
  }
  // Try all coins one by one
  for(int i = min_coin; i < N_COINS; i++)
  {
    coins[i]++;
    get_coins(total_val - denoms[i], coins, i);
    coins[i]--;
  }
}

int main()
{
  int coins[N_COINS];
  for(int i = 0; i < N_COINS; i++)
  {
    coins[i] = 0;
  }
  get_coins(51, coins, 0);
  return 0;
}
