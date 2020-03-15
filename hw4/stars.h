#ifndef STARS_H
#define STARS_H
enum StarType {MainSeq, Binary, VarLight};

struct MainSeqStars
{
  int age;
  char color[13];
};

struct BinaryStars
{
  double r1;
  double r2;
};

struct VarLightStars
{
  float lowestLum;
  float highestLum;
  int period;
};

union StarUnion
{
  struct MainSeqStars mss;
  struct BinaryStars bs;
  struct VarLightStars vls;
};

struct Star
{
  enum StarType type;
  union StarUnion data;
};

void print_star(struct Star);
void scan_star(struct Star *);
void error_invalid();

#endif /* end of include guard: STARS_H */
