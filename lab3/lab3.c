// Name sort
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
char name[2000][100];
int n= 0;

void bubble_sort()
{
  int sorted = 0;
  while(!sorted)
  {
    sorted = 1;
    for(int i = 0; i < n- 1; i++)
    {
      if(strcmp(name[i], name[i + 1]) >= 0)
      {
	char temp[100];
	strcpy(temp, name[i]);
	strcpy(name[i], name[i + 1]);
	strcpy(name[i + 1], temp);
	sorted = 0;
      }
    }
  }
}

void print_names()
{
    for(int i = 0; i < n; i++)
  {
    printf("%s\n", name[i]);
  }
}

int bin_search(char str[])
{
  // Returns the index or -1 if not found
  int i = 0;
  int j = n - 1;
  while(i <= j)
  {
    int mid = (i+j)/2;
    int diff = strcmp(name[mid], str);
    if(diff == 0)
    {
      // Found
      return mid;
    }else if( diff > 0)
    {
      // First half
      j = mid - 1;
    }else
    {
      // Second half
      i = mid + 1;
    }
  }
  return -1;
}

int main()
{
  FILE* fin = fopen("tp.csv", "r");
  if(fin == NULL)
  {
    printf("File not found");
    exit(1);
  }
  // Discard first line
  fscanf(fin, "%*[^\n]\n");
  while(!feof(fin))
  {
    fscanf(fin, "%*d;%[^;];%*f;%*d\n", name[n]);
    n++;
  }

  bubble_sort();

  char *input = NULL;
  size_t n = 0;
  do
  {
    printf("Insert a name of the form AAAAAAA AAAAAAA\n");
    getline(&input, &n, stdin);
  }while(strlen(input) != 16);
  
  // Discard the trailing \n
  input[15] = '\0';

  int i = bin_search(input);
  if(i != -1)
  {
    printf("String %s found at index %d!\n", input, i);
  }else
  {
    printf("String not found!\n");
  }
  free(input);
  fclose(fin);
}


