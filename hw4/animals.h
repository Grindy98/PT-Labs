#ifndef ANIMALS_H
#define ANIMALS_H

enum AnimalType {Insect, Bird, Mammal, Fish};

struct Insect
{
  int n_legs;
  int life_span;
};

struct Bird
{
  float flight_speed;
  float wing_length;
  char migration_habits[20];// Storing the migration_habits as a string
};

struct Mammal
{
  float weight;
  float height;
  char food_type[20];
};

struct Fish
{
  float weight;
  float swimming_depth;
  float water_salt_pct; // 0-1
};

union AnimalUnion
{
  struct Insect i;
  struct Bird b;
  struct Mammal m;
  struct Fish f;
};

struct Animal
{
  enum AnimalType type;
  union AnimalUnion data;
};

// Global vector representing the database
struct Animal ** data;
int n_data = 0, cap_data = 0;

void update_data_cap();
int add(int index, struct Animal a);
int delete(int index);
int replace(int index, struct Animal rep);
int lookup(struct Animal look);
int is_same_animal(struct Animal a1, struct Animal a2);

#endif /* end of include guard: ANIMALS_H */
