#include <stdio.h>
#include <stdlib.h>



int main()
{
  int change = 0;
  while(1)
  {
    printf("Please insert the change:");
    scanf("%d", &change);
    if(change > 0)
    {
      break;
    }
    // Ask for correct input
    printf("\nInput invalid, natural number required!\n");
  }

  // Had we not known the denominations beforehand, we would have had to sort this array
  int denoms[] = {20, 10, 5, 1};
  int n_denom = sizeof(denoms)/sizeof(denoms[0]);
  int partition[n_denom];
  for(int i = 0; i < n_denom; i++)
  {
    partition[i] = 0;
  }

  if(denoms[n_denom - 1] != 1)
  {
    // Safety check, we have to have 1 at the end
    printf("ERROR:1 has to be a denomination!\n");
    exit(1);
  }

  while(change > 0)
  {
    for(int i = 0; i < n_denom; i++)
    {
      if(change >= denoms[i])
      {
        // Found the biggest coin smaller than the sum
        change -= denoms[i];
        partition[i]++;
        break;
      }
    }
  }

  printf("Your change can be split into:\n");
  for(int i = 0; i < n_denom; i++)
  {
    if(partition[i] == 0)
    {
      continue;
    }
    printf("%d coin(s) of %d cent(s).\n", partition[i], denoms[i]);
  }
  return 0;
}
