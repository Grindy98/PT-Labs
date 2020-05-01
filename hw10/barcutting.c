#include <stdio.h>

int maxprofit_rec(const int barprice[], const int max_len_cut, int curr_len_cut, int length, int interm[][max_len_cut])
{
  // If curr_len_cut reached the end of the array
  if(curr_len_cut > max_len_cut)
  {
    return 0;
  }
  // If length is 0 you cannot cut further (efficiency)
  if(length == 0)
  {
    return 0;
  }
  // If previously computed value exists, use it
  if(interm[length][curr_len_cut - 1] != -1)
  {
    return interm[length][curr_len_cut - 1];
  }

  // Max value if skipping over this length cut
  int result = maxprofit_rec(barprice, max_len_cut, curr_len_cut + 1, length, interm);

  if(length - curr_len_cut >= 0)
  {
    // Cut is possible -- 2 options
    // Max value if cutting now and if we keep the current cut length
    int temp1 = barprice[curr_len_cut - 1] + maxprofit_rec(barprice, max_len_cut,
      curr_len_cut, length - curr_len_cut, interm);
    // Max value if cutting now and if we move to the next cut length
    int temp2 = barprice[curr_len_cut - 1] + maxprofit_rec(barprice, max_len_cut,
      curr_len_cut + 1, length - curr_len_cut, interm);

    // Store the maximum of all options
    result = result > temp1 ? result : temp1;
    result = result > temp2 ? result : temp2;
  }

  // Store the result for uses in the future
  interm[length][curr_len_cut - 1] = result;
  return result;
}

int maxprofit(const int barprice[], const int max_len_cut, int length)
{
  // Initialize the interm array
  int interm[length + 1][max_len_cut];
  for(int i = 0; i < length + 1; i++)
  {
    for(int j = 0; j < max_len_cut; j++)
    {
      interm[i][j] = -1;
    }
  }
  return maxprofit_rec(barprice, max_len_cut, 1, length, interm);
}

int main()
{
  // Price stored from 1 to whichever is the longest possible length cut
  // In this case between 1 and 6
  int barprice[] = {1, 5, 7, 10, 14, 17};
  int max_len_cut = sizeof(barprice)/sizeof(barprice[0]);
  printf("%d\n", maxprofit(barprice, max_len_cut, 8));
  return 0;
}
