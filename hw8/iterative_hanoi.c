#include <stdio.h>
#include <stdlib.h>

void recurs_hanoi(int n_disk, const char* fromPeg, const char* toPeg, const char* auxPeg)
{
  // Check if base case
  if(n_disk == 1)
  {
    printf("Move disk from %s to %s\n", fromPeg, toPeg);
  }else
  {
    recurs_hanoi(n_disk - 1, fromPeg, auxPeg, toPeg);
    printf("Move disk from %s to %s\n", fromPeg, toPeg);
    recurs_hanoi(n_disk - 1, auxPeg, toPeg, fromPeg);
  }
}

typedef struct STACK_NODE
{
  int n_disk;
  struct STACK_NODE* next;
} STACK_NODE;

void stack_push(STACK_NODE** s, int disk_push)
{
  STACK_NODE* new_node = malloc(sizeof(STACK_NODE));
  if(new_node == NULL)
  {
    printf("Not enough memory!\n");
    exit(1);
  }
  new_node->n_disk = disk_push;
  if(*s == NULL)
  {
    // First element
    new_node->next = NULL;
  }else
  {
    // Push in stack
    new_node->next = *s;
  }
  *s = new_node;
}

int stack_pop(STACK_NODE** s, int* disk_pop)
{
  if(*s == NULL)
  {
    return 0;
  }
  // Store the popped element and the next element
  *disk_pop = (*s)->n_disk;
  STACK_NODE* next_in_line = (*s)->next;
  // Pop the first element
  free(*s);
  (*s) = next_in_line;
  return 1;
}

// Return 0 if a -> b, 1 otherwise
int perform_move(STACK_NODE** a, STACK_NODE** b)
{
  if(*a == NULL)
  {
    if(*b == NULL)
    {
      // Should never happen
      printf("Can't move between 2 empty pegs!\n");
      exit(1);
    }
    // Move from b to a
    int n_disk;
    stack_pop(b, &n_disk);
    stack_push(a, n_disk);
    return 1;
  }
  if(*b == NULL)
  {
    // Move from a to b
    int n_disk;
    stack_pop(a, &n_disk);
    stack_push(b, n_disk);
    return 0;
  }
  // Both a and b have at least 1 disk -- move small disk on big disk
  if((*a)->n_disk < (*b)->n_disk)
  {
    int n_disk;
    stack_pop(a, &n_disk);
    stack_push(b, n_disk);
    return 0;
  }
  int n_disk;
  stack_pop(b, &n_disk);
  stack_push(a, n_disk);
  return 1;
}

void print_s(STACK_NODE* s)
{
  if(s == NULL)
  {
    return;
  }
  print_s(s->next);
  printf("%d ", s->n_disk);
}

int main()
{
  STACK_NODE* source = NULL;
  STACK_NODE* dest = NULL;
  STACK_NODE* aux = NULL;
  // Form the initial stack
  int number_initial_disks = 4;
  for(int i = number_initial_disks; i >= 1; i--)
  {
    stack_push(&source, i);
  }
  int steps = (1u << number_initial_disks) - 1;
  char* dest_string, * aux_string;
  if(number_initial_disks % 2 == 0)
  {
    // Interchange aux and dest
    dest_string = "Aux";
    aux_string = "Dest";
  }else
  {
    // Normal
    dest_string = "Dest";
    aux_string = "Aux";
  }
  for(int i = 1; i <= steps; i++)
  {
    if(i % 3 == 1)
    {
      // Perform move between Source and Dest
      int which = perform_move(&source, &dest);
      printf("Move disk from %s to %s\n",
      which ? dest_string : "Source", which ? "Source" : dest_string);
    }else if(i % 3 == 2)
    {
      // Perform move between Source and Aux
      int which = perform_move(&source, &aux);
      printf("Move disk from %s to %s\n",
      which ? aux_string : "Source", which ? "Source" : aux_string);
    }else
    {
      // Perform move between Aux and Dest
      int which = perform_move(&aux, &dest);
      printf("Move disk from %s to %s\n",
      which ? dest_string : aux_string, which ? aux_string : dest_string);
    }
  }
  if(number_initial_disks % 2 == 0)
  {
    // Interchange aux and dest pegs
    STACK_NODE* temp = aux;
    aux = dest;
    dest = temp;
  }
  print_s(source);
  printf("\n");
  print_s(aux);
  printf("\n");
  print_s(dest);
  printf("\n");
  return 0;
}
