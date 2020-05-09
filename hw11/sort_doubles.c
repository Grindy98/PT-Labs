#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int comp_doubles(const char** a1, const char** a2)
{
  // First check if both a1 and a2 are proper doubles in exponential form
  int exp_len;
  double f1, f2;
  sscanf(*a1, "%le%n", &f1, &exp_len);
  if(exp_len != strlen(*a1))
  {
    // First arg is not in exponential form
    fprintf(stderr, "%s is not in exponential form!\n", *a1);
    exit(1);
  }
  sscanf(*a2, "%le%n", &f2, &exp_len);
  if(exp_len != strlen(*a2))
  {
    // Second arg is not in exponential form
    fprintf(stderr, "%s is not in exponential form!\n", *a2);
    exit(1);
  }
  if(f1 == f2)
  {
    return 0;
  }
  if(f1 - f2 > 0)
  {
    return 1;
  }
  return -1;
}

int main(int argc, char* args[])
{
 if(argc < 2)
 {
   fprintf(stderr, "Usage:%s <double in exponential form>...\n", args[0]);
   exit(1);
 }
 qsort(args + 1, argc - 1, sizeof(char*), (int(*)(const void*, const void*))comp_doubles);
 printf("The arguments sorted in ascending order are:\n");
 for(int i = 1; i < argc; i++)
 {
   printf("%s ", args[i]);
 }
 printf("\n");
 return 0;
}
