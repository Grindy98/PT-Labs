
// Storing every element of the struct but sorting by name only

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct s
{
  unsigned index;
  char name[20];
  double grade;
  int birth_year;
} STUDENT;

STUDENT students[2000];
int n = 0;

void bubble_sort()
{
  int sorted = 0;
  STUDENT temp;
  while(!sorted)
  {
    sorted = 1;
    for(int i = 0; i < n- 1; i++)
    {
      if(strcmp(students[i].name, students[i + 1].name) >= 0)
      {
        temp = students[i];
        students[i] = students[i + 1];
        students[i + 1] = temp;
      	sorted = 0;
      }
    }
  }
}

void print_names()
{
    for(int i = 0; i < n; i++)
  {
    printf("%s\n", students[i].name);
  }
}

int bin_search(char find[])
{
  // Returns the index or -1 if not found
  int i = 0;
  int j = n - 1;
  // Sort only by name
  while(i <= j)
  {
    int mid = (i+j)/2;
    int diff = strcmp(students[mid].name, find);
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
    STUDENT *ptr = students + n;
    if(fscanf(fin, "%d;%[^;];%lf;%d\n", &ptr->index, ptr->name, &ptr->grade, &ptr->birth_year) != 4)
    {
      printf("Bad input at line %d!\n", n + 2);
      exit(1);
    }
    n++;
  }

  bubble_sort();

  // Get what to search
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
    printf("Student found at index %d!\n", i);
    printf("Full student details:\n");
    printf("Original list position: %u\n", students[i].index);
    printf("Name: %s\n", students[i].name);
    printf("Grade: %lf\n", students[i].grade);
    printf("Birth year: %u\n", students[i].birth_year);
  }else
  {
    printf("Student not found!\n");
  }
  free(input);
  fclose(fin);
}
