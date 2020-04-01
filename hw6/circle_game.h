#ifndef _CIRCLE_GAME_H
#define _CIRCLE_GAME_H
#include <string.h>
typedef struct Kid
{
  char* name;
  struct Kid* prev;
  struct Kid* next;
} KID;

KID* start = NULL,* end = NULL;

void initialize_list();
void add(char* name);
KID* add_between(char* name, KID* bef, KID* aft);
KID* lookup(char* name);
void delete(KID** kid, char* deleted_name);
void print_list();
void print_help();

#endif
