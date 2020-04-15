#include <stdio.h>
#include <stdlib.h>
#define OUT_FILE "output.svg"
#define SQUARE_SIZE 10

long mypow(long base, unsigned e)
{
  if(e == 0)
  {
    return 1;
  }
  long temp = mypow(base, e / 2);
  if(e % 2 == 0)
  {
    return temp * temp;
  }
  return base * temp * temp;
}

void sierpinsky(int c, long d_x, long d_y, FILE* fout)
{
  if(c <= 0)
  {
    // Base case, print out a new square
    fprintf(fout, "<rect x=\"%ld\" y=\"%ld\" width=\"%d\" height=\"%d\" fill=\"red\" />\n",
    d_x, d_y, SQUARE_SIZE, SQUARE_SIZE);
    return;
  }
  // Compute the specific displacement for the current level
  long disp = mypow(3, c - 1) * SQUARE_SIZE;
  c--;
  // Left side of the square
  sierpinsky(c, d_x, d_y, fout);
  sierpinsky(c, d_x, d_y + disp, fout);
  sierpinsky(c, d_x, d_y + 2 * disp, fout);

  // Middle side of the square
  sierpinsky(c, d_x + disp, d_y, fout);
  sierpinsky(c, d_x + disp, d_y + 2 * disp, fout);

  // Right side of the square
  sierpinsky(c, d_x + 2 * disp, d_y, fout);
  sierpinsky(c, d_x + 2 * disp, d_y + disp, fout);
  sierpinsky(c, d_x + 2 * disp, d_y + 2 * disp, fout);
}

void exit_help()
{
  printf("This program takes as an input parameter a positive integer as the number of iterations!\n");
  exit(1);
}

int main(int argc, char* argv[])
{
  if(argc != 2)
  {
    exit_help();
  }
  int c = -1;
  sscanf(argv[1], "%d", &c);
  if(c == -1)
  {
    exit_help();
  }
  FILE* fout = fopen(OUT_FILE, "w");
  if(fout == NULL)
  {
    printf("Cannot write an output file!\n");
    exit(1);
  }
  // Write the heading
  fprintf(fout, "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" width=\"500\" height=\"500\" ");
  long full_size = mypow(3, c) * SQUARE_SIZE;
  fprintf(fout, "viewBox = \"0 0 %ld %ld\" >\n", full_size, full_size);

  sierpinsky(c, 0, 0, fout);

  // Write the ending
  fprintf(fout, "</svg>\n");
  return 0;
}
