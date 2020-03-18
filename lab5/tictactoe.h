#ifndef TICTACTOE_H
#define TICTACTOE_H

#define GRID_SIZE 3
char grid[GRID_SIZE][GRID_SIZE];
// Initial empty tiles
int empty = GRID_SIZE * GRID_SIZE;

typedef struct s
{
  int i;
  int j;
}IND;

IND get_empty_pos();
void place(char player, IND pos);
void print_grid();
int check_end(IND pos);
int perform_random_move(char player);

#endif
