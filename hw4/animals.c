#include "animals.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main()
{
  data = NULL;
  // Initial alloc
  update_data_cap();

  struct Animal a1, a2, a3, a4, a5;

  // Assigning data to each animal
  a1.type = Insect;
  a1.data.i.n_legs = 8;
  a1.data.i.life_span = 100;

  a2.type = Bird;
  a2.data.b.flight_speed = 10.0f;
  a2.data.b.wing_length = 25.4f;
  strcpy(a2.data.b.migration_habits, "Migratory");

  a3.type = Mammal;
  a3.data.m.weight = 0.5f;
  a3.data.m.height = 10.4f;
  strcpy(a3.data.m.food_type, "Grains");

  a4.type = Fish;
  a4.data.f.weight = 700.0f;
  a4.data.f.swimming_depth = 50.0f;
  a4.data.f.water_salt_pct = 0.03f;

  a5.type = Fish;
  a5.data.f.weight = 5.4f;
  a5.data.f.swimming_depth = 0.5f;
  a5.data.f.water_salt_pct = 0.0f;

  add(0, a1);
  add(0, a2);
  add(0, a3);
  add(0, a4);
  add(0, a5);
  add(3, a1);
  printf("%d\n", n_data);
  printf("%d\n", lookup(a1));
  return 0;
}

int add(int index, struct Animal a)
{
  // Returns index on success and -1 on failure
  if(index > n_data)
  {
    // Out of bounds
    return -1;
  }
  struct Animal *new_s_ptr = malloc(sizeof(struct Animal));
  if(new_s_ptr == NULL)
  {
    printf("Error - not enough memory\n");
    exit(1);
  }
  // Copy the struct into a new memory location
  *new_s_ptr = a;
  // Classic insertion in vector
  for(int i = n_data; i > index; i--)
  {
    data[i] = data[i - 1];
  }
  data[index] = new_s_ptr;
  n_data++;
  update_data_cap();
  return index;
}

int delete(int index)
{
  if(index >= n_data)
  {
    // Out of bounds
    return -1;
  }
  // Free the memory allocated first
  free(data[index]);

  for(int i = index; i < n_data - 1; i++)
  {
    data[i] = data[i + 1];
  }
  n_data--;
  update_data_cap();
  return index;
}

int replace(int index, struct Animal rep)
{
  if(index >= n_data)
  {
    // Out of bounds
    return -1;
  }
  *data[index] = rep;
  return index;
}

int lookup(struct Animal look)
{
  // Perform a linear search
  for(int i = 0; i < n_data; i++)
  {
    if(is_same_animal(*data[i], look))
    {
      return i;
    }
  }
  // Not found
  return -1;
}

int is_same_animal(struct Animal a1, struct Animal a2)
{
  if(a1.type != a2.type)
  {
    return 0;
  }
  switch(a1.type)
  {
    case Insect:
      return a1.data.i.n_legs == a2.data.i.n_legs &&
      a1.data.i.life_span == a2.data.i.life_span;
    case Bird:
      return a1.data.b.flight_speed == a2.data.b.flight_speed &&
      a1.data.b.wing_length == a2.data.b.wing_length &&
      strcmp(a1.data.b.migration_habits, a2.data.b.migration_habits) == 0;
    case Mammal:
      return a1.data.m.weight == a2.data.m.weight &&
      a1.data.m.height == a2.data.m.height &&
      strcmp(a1.data.m.food_type, a2.data.m.food_type) == 0;
    case Fish:
      return a1.data.f.weight == a2.data.f.weight &&
      a1.data.f.swimming_depth == a2.data.f.swimming_depth &&
      a1.data.f.water_salt_pct == a2.data.f.water_salt_pct;
    default:
      return 0;
  }
}

void update_data_cap()
{
  // Buffer of 2
  if(n_data + 2 >= cap_data)
  {
    // Add memory
    cap_data += 10;
    if((data = realloc(data, sizeof(struct Animal *) * cap_data)) == NULL)
    {
      printf("Error - not enough memory\n");
      exit(1);
    }
  }else if(n_data + 12 <= cap_data )
  {
    // Remove memory - no need to check for errors
    cap_data -= 10;
    data = realloc(data, sizeof(struct Animal *) * cap_data);
  }
}
