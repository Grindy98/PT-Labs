#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "stars.h"

int main()
{
  struct Star s;
  scan_star(&s);
  print_star(s);
  return 0;
}

void scan_star(struct Star *s)
{
  printf("Type the star name to be read.\n");
  printf("[b for Binary|m for Main Sequence|v for Variable Light]:");
  char name = '\0';
  name = getchar();
  if(name == 'm')
  {
    s->type = MainSeq;
  }else if(name == 'b')
  {
    s->type = Binary;
  }else if(name == 'v')
  {
    s->type = VarLight;
  }else
  {
    // Bad input
    error_invalid();
  }
  switch(s->type)
  {
    case MainSeq:
    {
      printf("Insert the data [age_category color]:\n");
      int age = 0;
      char color[13];
      if(scanf("%d", &age) != 1)
      {
        error_invalid();
      }
      if(age < 1 || age > 4)
      {
        error_invalid();
      }
      if(scanf("%12s", color) != 1)
      {
        error_invalid();
      }
      s->data.mss.age = age;
      strcpy(s->data.mss.color, color);
      break;
    }
    case Binary:
    {
      printf("Insert the data [radius1 radius2]:\n");
      double r1, r2;
      if(scanf("%lf", &r1) != 1)
      {
        error_invalid();
      }
      if(scanf("%lf", &r2) != 1)
      {
        error_invalid();
      }
      s->data.bs.r1 = r1;
      s->data.bs.r2 = r2;
      break;
    }
    case VarLight:
    {
      printf("Insert the data [lowest_luminosity highest_luminosity period]:\n");
      float lowestLum, highestLum;
      int period;
      if(scanf("%f", &lowestLum) != 1)
      {
        error_invalid();
      }
      if(scanf("%f", &highestLum) != 1)
      {
        error_invalid();
      }
      if(scanf("%d", &period) != 1)
      {
        error_invalid();
      }
      s->data.vls.lowestLum = lowestLum;
      s->data.vls.highestLum = highestLum;
      s->data.vls.period = period;
      break;
    }
  }
}

void print_star(struct Star s)
{
  switch(s.type)
  {
    case MainSeq:
    {
      printf("The read star is of type Main Sequence with:\n");
      printf("Age category %d and of color %s.\n", s.data.mss.age,
      s.data.mss.color);
      break;
    }
    case Binary:
    {
      printf("The read star is of type Binary with:\n");
      printf("Radius 1 of %lf and radius 2 of %lf.\n", s.data.bs.r1,
      s.data.bs.r2);
      break;
    }
    case VarLight:
    {
      printf("The read star is of type Variable Light with:\n");
      printf("Luminosity between %f and %f, \n", s.data.vls.lowestLum,
      s.data.vls.highestLum);
      printf(" with a period of %d.\n", s.data.vls.period);
      break;
    }
  }
}

void error_invalid()
{
  printf("Invalid input!\n");
  exit(1);
}
