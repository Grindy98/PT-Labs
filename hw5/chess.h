#ifndef _CHESS_H
#define _CHESS_H
#define BOARD_SIZE 8

typedef enum PieceType {Pawn, Knight, Bishop, Rook, Queen, King} PIECETYPE;
typedef enum Outcome {GameStillGoing, GameLost, KingCheck, Stalemate} OUTCOME;

struct PieceStruct;
struct TileStruct;

typedef struct
{
  int i;
  int j;
} POS;

typedef struct TileStruct
{
  POS pos;
  struct PieceStruct* piece;
  // n -> none, w -> white, b -> black, a -> all
  char under_attack;
} TILE;

typedef struct PieceStruct
{
  struct TileStruct* tile;
  PIECETYPE type;
  int is_white;
} PIECE;

typedef struct
{
  const struct PieceStruct* piece_to_move;
  int dest_n;
  POS *dest_arr;
} POSSIBLEMOVES;

TILE board[BOARD_SIZE][BOARD_SIZE];


int move_piece(PIECE* source, TILE* dest, PIECE* removed_piece);

POSSIBLEMOVES get_moves_pawn(const PIECE* piece);
POSSIBLEMOVES get_moves_knight(const PIECE* piece);
POSSIBLEMOVES get_moves_r_b_q(const PIECE* piece);
POSSIBLEMOVES get_moves_king(const PIECE* piece);
POSSIBLEMOVES get_moves(const PIECE* piece);
POSSIBLEMOVES* get_all_team_moves(int* n, int is_white);

OUTCOME perform_random_turn(int is_white, int* piece_eliminated);
int game_is_draw();
PIECE* get_king(int is_white);
void replace_pawn(PIECE* curr_pawn);

int tile_empty(POS pos);
int tile_occed_by_piece(POS pos, int is_white);
void attack_tile(POS pos, int is_white);
void reset_attacks();
void recalculate_attacks(int is_white);
int is_attacked_by(POS pos, int is_white);

TILE* get_tile(POS pos);
int out_of_bounds(POS pos);
void free_pointer(POSSIBLEMOVES** pm, int n);
PIECE* create_piece(PIECE prototype);
void remove_potential_move(POSSIBLEMOVES* pm, int index);
void initialize_board();

void print_board();

#endif
