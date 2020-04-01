#include "circle_game.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int main(int args, char* argv[])
{
  if(args != 3)
  {
    printf("Invalid number of arguments!\n");
    print_help();
    return 1;
  }
  int N = 0, c = 0;
  sscanf(argv[1], "%d", &N);
  sscanf(argv[2], "%d", &c);
  if(N <= 0)
  {
    printf("Invalid N!\n");
    print_help();
    return 1;
  }
  if(c != 1 && c != -1)
  {
    printf("Invalid c!\n");
    print_help();
    return 1;
  }
  initialize_list();
  printf("Kids at the start of the game:\n");
  print_list();
  int round_number = 1;
  KID* curr_kid;
  if(c == 1)
  {
    // Beginning from start
    curr_kid = start;
  }else
  {
    // Beginning from end
    curr_kid = end;
  }
  while(1)
  {
    // Game is running
    for(int i = 1; i < N; i++)
    {
      // Go to next or prev, depending on c
      if(c == 1)
      {
        curr_kid = curr_kid->next;
      }else
      {
        curr_kid = curr_kid->prev;
      }
    }
    // Reached the kid which has to be removed
    char removed_name[30];
    delete(&curr_kid, removed_name);
    printf("\n%s got removed from the circle in round #%d!\n",
    removed_name, round_number);
    if(curr_kid == NULL)
    {
      // Reached the end of the game
      break;
    }
    if(c == -1)
    {
      // If counterclockwise, then curr_kid has to be adjusted to start from prev
      curr_kid = curr_kid->prev;
    }
    print_list();
    round_number++;
  }
  printf("\nThere is no one in the circle -- Game Over!\n");
  return 0;
}

void initialize_list()
{
  add("Katherine");
  add("Nathan");
  add("Maria");
  add("John");
  add("Zya");
  add("Andrew");
  add("Carl");
  add("Fred");
  add("Markus");
}

void add(char* name)
{
  if(lookup(name) != NULL)
  {
    printf("Cannot add duplicates\n");
    exit(1);
  }
  if(start == NULL)
  {
    // Add first element of the list
    KID* new_k = malloc(sizeof(KID));
    if(new_k == NULL)
    {
      printf("Not enough memory!\n");
      exit(1);
    }
    new_k->name = strdup(name);
    new_k->next = new_k->prev = new_k;
    start = end = new_k;
    return;
  }
  KID* curr = start;
  // If before first element
  if(strcmp(name, start->name) < 0)
  {
    start = add_between(name, end, start);
    return;
  }else if(strcmp(name, end->name) > 0)
  {
    // If after last element
    end = add_between(name, end, start);
    return;
  }
  // Search for first element smaller than kid
  while(curr->next != start)
  {
    if(strcmp(name, curr->next->name) < 0)
    {
      // Insert kid here
      add_between(name, curr, curr->next);
      return;
    }
    curr = curr->next;
  }
  // Should never reach this point
  printf("Error inserting kid %s\n", name);
  exit(1);
}

KID* add_between(char* name, KID* bef, KID* aft)
{
  if(bef != aft->prev || aft != bef->next)
  {
    printf("Function only takes consecutive kids!\n");
    exit(1);
  }
  KID* new_k = malloc(sizeof(KID));
  if(new_k == NULL)
  {
    printf("Not enough memory!\n");
    exit(1);
  }
  new_k->name = strdup(name);
  new_k->next = aft;
  new_k->prev = bef;
  aft->prev = new_k;
  bef->next = new_k;
  return new_k;
}

KID* lookup(char* name)
{
  if(start == NULL)
  {
    return NULL;
  }
  KID* curr = start;
  do
  {
    int c = strcmp(name, curr->name);
    if(c == 0)
    {
      return curr;
    }else if(c < 0)
    {
      // Name should have been found by now
      return NULL;
    }
    curr = curr->next;
  }while(curr != start);
  return NULL;
}

void delete(KID** kid, char* deleted_name)
{
  if(*kid == NULL || lookup((*kid)->name) == NULL)
  {
    printf("Function has to delete a kid!\n");
    exit(1);
  }
  if(deleted_name != NULL)
  {
    // The user wants the name of the kid
    strcpy(deleted_name, (*kid)->name);
  }
  if(start == end)
  {
    // We know kid is in list and the list only has a single element =>
    // List is empty
    start = NULL;
    end = NULL;
    // Free the allocated memory
    free((*kid)->name);
    free((*kid));
    *kid = NULL;
    return;
  }
  if((*kid) == start)
  {
    // Start of list changed
    start = (*kid)->next;
  }else if((*kid) == end)
  {
    // End of list changed
    end = (*kid)->prev;
  }
  // Eliminate kid by relinking the prev and next elements
  (*kid)->next->prev = (*kid)->prev;
  (*kid)->prev->next = (*kid)->next;

  // Remember the NEXT kid to change the pointer at the end
  KID* next_k = (*kid)->next;
  // Free the allocated memory
  free((*kid)->name);
  free((*kid));
  *kid = next_k;
}

void print_list()
{
  if(start == NULL)
  {
    printf("There are no kids in the circle!\n");
    return;
  }
  KID* curr = start;
  int i = 1;
  printf("\n");
  do
  {
    printf("Kid #%d: %s\n", i, curr->name);
    i++;
    curr = curr->next;
  }while(curr != start);
}

void print_help()
{
  printf("The function takes as arguments ./a.out [N] [c]\n");
  printf("N - positive number chosen representing the number of skips.\n");
  printf("c - 1 if iterating clockwise and -1 if counterclockwise.\n");
}
