#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct ListElem
{
  char* name;
  int age;
  float grade;
  struct ListElem* next;
} LISTELEM;

LISTELEM* list_start = NULL;

void add(char name[], int age, float grade)
{
  // Dinamically allocate the memory
  LISTELEM* new_e = malloc(sizeof(LISTELEM));
  if(new_e == NULL)
  {
    printf("No memory");
    exit(1);
  }
  *new_e = (LISTELEM){strdup(name), age, grade, NULL};

  if(list_start == NULL)
  {
    // Add first element
    list_start = new_e;
    return;
  }
  LISTELEM* curr = list_start;
  // If before first element
  if(strcmp(new_e->name, curr->name) < 0)
  {
    new_e->next = list_start;
    list_start = new_e;
    return;
  }else if(strcmp(new_e->name, curr->next->name) == 0)
  {
    printf("Name %s already in list\n", new_e->name);
    exit(1);
  }
  // Search for first element smaller than new_e
  while(curr->next != NULL)
  {
    if(strcmp(new_e->name, curr->next->name) < 0)
    {
      // Insert new_e here
      new_e->next = curr->next;
      curr->next = new_e;
      return;
    }else if(strcmp(new_e->name, curr->next->name) == 0)
    {
      printf("Name %s already in list\n", new_e->name);
      exit(1);
    }
    curr = curr->next;
  }
  // If reached this point, should insert new_e last
  curr->next = new_e;
}

LISTELEM* find_prev(char name[], int* is_first)
{
  *is_first = 0;
  if(list_start == NULL)
  {
    return NULL;
  }
  LISTELEM* curr = list_start;
  if(strcmp(name, curr->name) == 0)
  {
    // Has no prev element bc it is first
    *is_first = 1;
    return NULL;
  }
  while(curr->next != NULL)
  {
    if(strcmp(name, curr->next->name) == 0)
    {
      return curr;
    }
    curr = curr->next;
  }
  // Not found
  return NULL;
}
LISTELEM* find(char name[])
{
  if(list_start == NULL)
  {
    return NULL;
  }
  LISTELEM* curr = list_start;
  while(curr != NULL)
  {
    if(strcmp(name, curr->name) == 0)
    {
      return curr;
    }
    curr = curr->next;
  }
  // Not found
  return NULL;
}

int delete(char name[])
{
  int is_first;
  LISTELEM* prev = find_prev(name, &is_first);
  if(is_first)
  {
    LISTELEM* temp_list_start = list_start->next;
    free(list_start->name);
    free(list_start);
    list_start = temp_list_start;
    return 1;
  }
  if(prev == NULL)
  {
    // Delete impossible
    return 0;
  }
  LISTELEM* temp_e = prev->next->next;
  free(prev->next->name);
  free(prev->next);
  prev->next = temp_e;
  return 1;
}

int modify_grade(char name[], float new_grade)
{
  LISTELEM* found = find(name);
  if(found == NULL)
  {
    return 0;
  }
  found->grade = new_grade;
  return 1;
}

void show()
{
  LISTELEM* curr = list_start;
  while(curr != NULL)
  {
    printf("Name:%s, Age:%d, Grade:%f\n", curr->name, curr->age, curr->grade);
    curr = curr->next;
  }
  printf("\n");
}

int main()
{
  add("Zai", 14, 7.8);
  add("Carl", 2, 9.2);
  add("Napoleon", 7, 5.5);
  add("Alissa", 10, 9.5);
  show();
  delete("Napoleon");
  add("Mark", 7, 9);
  modify_grade("Zai", 8.75);
  show();
  return 0;
}
