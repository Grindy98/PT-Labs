#include "chess.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
  srand(time(NULL));
  initialize_board();
  printf("AutoChess game!\nPress Enter after a move to proceed\n");
  printf("Press q to exit the program\n");
  if(getchar() == 'q')
  {
    return 0;
  }

  int is_white_turn = 1;
  int turns_until_draw = 50;
  while(turns_until_draw > 0)
  {
    int was_piece_eliminated;
    print_board();
    switch(perform_random_turn(is_white_turn, &was_piece_eliminated))
    {
      case GameStillGoing:
        printf("\n");
        break;
      case GameLost:
        printf("\n Game ended with the %s player as the winner!\n",
        is_white_turn ? "black":"white");
        return 0;
      case KingCheck:
        printf("\n %s king is in check!",
        is_white_turn ? "White":"Black");
        break;
      case Stalemate:
        printf("\n %s player can't move, game ended with a stalemate!\n",
        is_white_turn ? "White":"Black");
        return 0;
    }
    if(getchar() == 'q')
    {
      return 0;
    }
    if(was_piece_eliminated)
    {
      // Reset the draw counter if piece was eliminated this turn
      turns_until_draw = 51;
    }
    is_white_turn = !is_white_turn;
    turns_until_draw--;
  }
  printf("\n Game ended with a draw!\n");
  return 0;
}

int move_piece(PIECE* piece, TILE* to_tile, PIECE* removed_piece)
{
  if(piece == NULL || to_tile == NULL)
  {
    printf("Null pointer sent to move_piece\n");
    exit(1);
  }
  // Piece moved on free tile
  if(to_tile->piece == NULL)
  {
    to_tile->piece = piece;
    // Old tile is empty
    piece->tile->piece = NULL;
    piece->tile = to_tile;
    return 0;
  }
  if(to_tile->piece->is_white == piece->is_white)
  {
    printf("Invalid move, cannot eliminate your own piece");
    exit(1);
  }
  // Piece eliminated other piece
  *removed_piece = *(to_tile->piece);
  free(to_tile->piece);
  to_tile->piece = piece;
  // Old tile is empty
  piece->tile->piece = NULL;
  piece->tile = to_tile;
  return 1;
}

POSSIBLEMOVES get_moves_pawn(const PIECE* piece)
{
  if(piece->type != Pawn)
  {
    printf("Not a pawn\n");
    exit(1);
  }
  int n = 0;
  POS* arr = malloc(sizeof(POS) * 5);
  int i = piece->tile->pos.i;
  int j = piece->tile->pos.j;
  // Move up/down depending on start position and color
  // Exception: If initial position, pawn can move 2 tiles
  if(piece->is_white)
  {
    if(i == 1 && tile_empty((POS){i + 2, j}))
    {
      arr[n++] = (POS){i + 2, j};
    }
    i++;
  }else
  {
    if(i == 6 && tile_empty((POS){i - 2, j}))
    {
      arr[n++] = (POS){i - 2, j};
    }
    i--;
  }
  // Add characteristic pawn moves
  if(tile_empty((POS){i, j}) == 1)
  {
    arr[n++] = (POS){i, j};
  }
  // Attack right, and potential move if enemy
  attack_tile((POS){i, j + 1}, piece->is_white);
  if(tile_occed_by_piece((POS){i, j + 1}, !piece->is_white))
  {
    arr[n++] = (POS){i, j + 1};
  }
  // Attack left, and potential move if enemy
  attack_tile((POS){i, j - 1}, piece->is_white);
  if(tile_occed_by_piece((POS){i, j - 1}, !piece->is_white))
  {
    arr[n++] = (POS){i, j - 1};
  }
  arr = realloc(arr, sizeof(POS) * n);
  return (POSSIBLEMOVES){piece, n, arr};
}

POSSIBLEMOVES get_moves_knight(const PIECE* piece)
{
  if(piece->type != Knight)
  {
    printf("Not a knight\n");
    exit(1);
  }
  int n = 0;
  POS* arr = malloc(sizeof(POS) * 10);
  int i = piece->tile->pos.i;
  int j = piece->tile->pos.j;
  POS potential_pos[8] =
  {
    (POS){i + 2, j + 1},
    (POS){i + 1, j + 2},
    (POS){i - 1, j + 2},
    (POS){i - 2, j + 1},
    (POS){i - 2, j - 1},
    (POS){i - 1, j - 2},
    (POS){i + 1, j - 2},
    (POS){i + 2, j - 1},
  };
  for(int i = 0; i < 8; i++)
  {
    if(!out_of_bounds(potential_pos[i]))
    {
      // If in bounds, always attack tile
      attack_tile(potential_pos[i], piece->is_white);
    }
    // If tile empty or enemy present, possible move
    if(tile_empty(potential_pos[i]) ||
    tile_occed_by_piece(potential_pos[i], !piece->is_white))
    {
      arr[n++] = potential_pos[i];
    }
  }
  arr = realloc(arr, sizeof(POS) * n);
  return (POSSIBLEMOVES){piece, n, arr};
}

// ONLY USED IN MOVE ROOK, BISHOP, QUEEN FUNCTS
POS alter_pos_valid_moves(POS pos, const PIECE* piece, int dir)
{
  // Only works for rook, bishop, queen
  switch(piece->type)
  {
    case Rook:
    {
      // 0 -> i++ | 1 -> j++ | 2 -> i-- | 3 -> j--
      switch(dir)
      {
        case 0:
          pos.i++;
          break;
        case 1:
          pos.j++;
          break;
        case 2:
          pos.i--;
          break;
        case 3:
          pos.j--;
          break;
        default:
          printf("Invalid dir\n");
          exit(1);
      }
      return pos;
    }
    case Bishop:
    {
      // 0 -> i++ j++ | 1 -> i-- j++ | 2 -> i-- j-- | 3 -> i++ j--
      switch(dir)
      {
        case 0:
          pos.i++;
          pos.j++;
          break;
        case 1:
          pos.i--;
          pos.j++;
          break;
        case 2:
          pos.i--;
          pos.j--;
          break;
        case 3:
          pos.i++;
          pos.j--;
          break;
        default:
          printf("Invalid dir\n");
          exit(1);
      }
      return pos;
    }
    case Queen:
    {
      // 0 -> i++     | 1 -> j++     | 2 -> i--     | 3 -> j--
      // 4 -> i++ j++ | 5 -> i-- j++ | 6 -> i-- j-- | 7 -> i++ j--
      switch(dir)
      {
        case 0:
          pos.i++;
          break;
        case 1:
          pos.j++;
          break;
        case 2:
          pos.i--;
          break;
        case 3:
          pos.j--;
          break;
        case 4:
          pos.i++;
          pos.j++;
          break;
        case 5:
          pos.i--;
          pos.j++;
          break;
        case 6:
          pos.i--;
          pos.j--;
          break;
        case 7:
          pos.i++;
          pos.j--;
          break;
        default:
          printf("Invalid dir\n");
          exit(1);
      }
      return pos;
    }
    default:
      printf("Function can only be used on Rook, Bishop, Queen\n");
      exit(1);
  }
}
// ONLY USED IN MOVE ROOK, BISHOP, QUEEN FUNCTS
void get_moves_recurs(POS curr_pos, POSSIBLEMOVES* pm, int dir)
{
  if(out_of_bounds(curr_pos))
  {
    return;
  }

  const PIECE* original = pm->piece_to_move;
  PIECE* potenital_piece = get_tile(curr_pos)->piece;
  // Attack current tile
  attack_tile(curr_pos, original->is_white);
  if(potenital_piece != NULL && potenital_piece->is_white == original->is_white)
  {
    // If same team piece
    return;
  }
  // Add it to the set of tiles where you can move to
  pm->dest_arr[pm->dest_n] = curr_pos;
  pm->dest_n++;
  if(potenital_piece == NULL)
  {
    // Only if this was an empty tile, keep going
    curr_pos = alter_pos_valid_moves(curr_pos, original, dir);
    get_moves_recurs(curr_pos, pm, dir);
  }
}
POSSIBLEMOVES get_moves_r_b_q(const PIECE* piece)
{
  POS* arr = malloc(sizeof(POS) * 50);
  POSSIBLEMOVES pm = {piece, 0, arr};
  POS init_pos = piece->tile->pos;

  if(piece->type == Rook || piece->type == Bishop)
  {
    // 4 directions
    for(int i = 0; i < 4; i++)
    {
      POS new_pos = alter_pos_valid_moves(init_pos, piece, i);
      get_moves_recurs(new_pos, &pm, i);
    }
  }else if(piece->type == Queen)
  {
    // 8 directions
    for(int i = 0; i < 8; i++)
    {
      POS new_pos = alter_pos_valid_moves(init_pos, piece, i);
      get_moves_recurs(new_pos, &pm, i);
    }
  }else
  {
    printf("Piece must be a rook, bishop or queen\n");
    exit(1);
  }

  pm.dest_arr = realloc(pm.dest_arr, sizeof(POS) * pm.dest_n);
  return pm;
}

POSSIBLEMOVES get_moves_king(const PIECE* piece)
{
  if(piece->type != King)
  {
    printf("Not a king\n");
    exit(1);
  }
  int n = 0;
  POS* arr = malloc(sizeof(POS) * 10);
  int i = piece->tile->pos.i;
  int j = piece->tile->pos.j;
  POS potential_pos[8] =
  {
    (POS){i + 1, j    },
    (POS){i + 1, j + 1},
    (POS){i    , j + 1},
    (POS){i - 1, j + 1},
    (POS){i - 1, j    },
    (POS){i - 1, j - 1},
    (POS){i    , j - 1},
    (POS){i + 1, j - 1},
  };

  for(int i = 0; i < 8; i++)
  {
    if(!out_of_bounds(potential_pos[i]))
    {
      // If in bounds, always attack tile
      attack_tile(potential_pos[i], piece->is_white);
    }
    // If tile empty or enemy present AND tile not attacked, possible move
    if(tile_empty(potential_pos[i]) ||
    tile_occed_by_piece(potential_pos[i], !piece->is_white))
    {
      // If NOT attacked by ENEMY, only then consider it a possible move
      if(!is_attacked_by(potential_pos[i], !piece->is_white))
      {
        arr[n++] = potential_pos[i];
      }
    }
  }
  arr = realloc(arr, sizeof(POS) * n);
  return (POSSIBLEMOVES){piece, n, arr};
}

POSSIBLEMOVES get_moves(const PIECE* piece)
{
  switch(piece->type)
  {
    case Pawn:
      return get_moves_pawn(piece);
    case Rook:
      return get_moves_r_b_q(piece);
    case Knight:
      return get_moves_knight(piece);
    case Bishop:
      return get_moves_r_b_q(piece);
    case Queen:
      return get_moves_r_b_q(piece);
    case King:
      return get_moves_king(piece);
    default:
      printf("Should never happen\n");
      exit(1);
  }
}

POSSIBLEMOVES* get_all_team_moves(int* n, int is_white)
{
  // Each arr element represents all possible moves of a piece
  POSSIBLEMOVES* arr = malloc(sizeof(POSSIBLEMOVES) * 20);
  *n = 0;
  // Iterate over the board to get each piece
  for (int i = 0; i < BOARD_SIZE; i++)
  {
    for (int j = 0; j < BOARD_SIZE; j++)
    {
      const PIECE* piece = board[i][j].piece;
      // Piece exists and is in the right team
      if(piece != NULL)
      {
        if(piece->is_white == is_white)
        {
          arr[*n] = get_moves(piece);
          *n = *n + 1;
        }
      }
    }
  }
  return realloc(arr, sizeof(POSSIBLEMOVES) * (*n));
}

OUTCOME perform_random_turn(int is_white, int* was_piece_eliminated)
{
  *was_piece_eliminated = 0;
  // Perform enemy attacks
  recalculate_attacks(!is_white);
  // Verify if king is in check
  PIECE* king = get_king(is_white);
  if(is_attacked_by(king->tile->pos, !king->is_white))
  {
    // The king has to move to a valid position
    POSSIBLEMOVES king_pm = get_moves_king(king);
    while(1)
    {
      // Repeat until king is not in check or no moves left
      if(king_pm.dest_n == 0)
      {
        // Game over, whoever's turn it is lost
        free(king_pm.dest_arr);
        return GameLost;
      }else
      {
        // Move the king to a randomly chosen tile
        int i = rand() % king_pm.dest_n;
        PIECE removed_piece;
        PIECE original_king = *king;
        *was_piece_eliminated = move_piece(king,
        get_tile(king_pm.dest_arr[i]), &removed_piece);
        // Check if it is a valid position
        recalculate_attacks(!is_white);
        if(is_attacked_by(king->tile->pos, !king->is_white))
        {
          // Repeat the while without moves
          remove_potential_move(&king_pm, i);
          // Reload everything
          king->tile->piece = NULL;
          free(king);
          king = create_piece(original_king);
          if(*was_piece_eliminated == 1)
          {
            // Recreate removed piece
            create_piece(removed_piece);
            *was_piece_eliminated = 0;
          }
        }else
        {
          // Only if king is for sure not in check at the end of the turn, proceed
          free(king_pm.dest_arr);
          return KingCheck;
        }
      }
    }
  }

  // Get your own team's moves and choose one at random
  int n = 0;
  POSSIBLEMOVES* arr = get_all_team_moves(&n, is_white);
  while(1)
  {
    // Repeat until king is not in check or no moves left

    // Delete pieces without possible moves
    for(int i = 0; i < n; i++)
    {
      if(arr[i].dest_n == 0)
      {
        free(arr[i].dest_arr);
        n--;
        for(int j = i; j < n; j++)
        {
          arr[j] = arr[j + 1];
        }
        i--;
      }
    }
    if(n == 0)
    {
      // Stalemate condition
      return Stalemate;
    }
    // Chosen board piece to move at random, ignore pieces that can't move
    int index = rand() % n;
    POSSIBLEMOVES* pm = &arr[index];

    // Chosen random move of the board piece -- we know dest_n != 0
    index = rand() % pm->dest_n;

    // Perform random move
    PIECE removed_piece;
    PIECE* moved_piece = (PIECE*)pm->piece_to_move;
    PIECE original_piece = *moved_piece;
    *was_piece_eliminated = move_piece(moved_piece,
    get_tile(pm->dest_arr[index]), &removed_piece);
    // Check if it is a valid position
    recalculate_attacks(!is_white);
    if(is_attacked_by(king->tile->pos, !king->is_white))
    {
      // Remove this move from the array
      remove_potential_move(pm, index);

      // Reload everything
      moved_piece->tile->piece = NULL;
      free(moved_piece);
      pm->piece_to_move = create_piece(original_piece);
      if(*was_piece_eliminated)
      {
        // Recreate removed piece
        *was_piece_eliminated = 0;
        create_piece(removed_piece);
      }
    }else
    {
      // Only if king is not in check after piece was moved, return normally
      // And check if piece was pawn and reached the end of the board
      if(moved_piece->type == Pawn)
      {
        if((moved_piece->is_white && moved_piece->tile->pos.i == 7) ||
        (!moved_piece->is_white && moved_piece->tile->pos.i == 0))
        {
          // Reached the corresponding end
          replace_pawn(moved_piece);
          printf("\nReplaced %s pawn with eliminated piece",
          moved_piece->is_white ? "white":"black");
        }
      }
      free_pointer(&arr, n);
      return GameStillGoing;
    }
  }
}

PIECE* get_king(int is_white)
{
  for (int i = 0; i < BOARD_SIZE; i++)
  {
    for (int j = 0; j < BOARD_SIZE; j++)
    {
      PIECE* piece = board[i][j].piece;
      if(piece != NULL)
      {
        if(piece->type == King && piece->is_white == is_white)
        {
          // Found the king
          return piece;
        }
      }
    }
  }
  printf(" %s king does not exist\n", is_white ? "White":"Black");
  exit(0);
}

void replace_pawn(PIECE* curr_pawn)
{
  if(curr_pawn->type != Pawn)
  {
    printf("Can only substitute pawns");
    exit(1);
  }
  int removed_pieces[6] = {8, 2, 2, 2, 1, 1};
  for (int i = 0; i < BOARD_SIZE; i++)
  {
    for (int j = 0; j < BOARD_SIZE; j++)
    {
      // Check if pieces are on same team
      if(board[i][j].piece != NULL)
      {
        if(board[i][j].piece->is_white == curr_pawn->is_white)
        {
          // Subtract from the initial pieces;
          removed_pieces[board[i][j].piece->type]--;
        }
      }
    }
  }
  int n = 0;
  PIECETYPE potential_replacements[4];
  for(int i = 1; i <5; i++)
  {
    // Can only change into rook/bishop/knight/queen if the piece is removed
    if(removed_pieces[i] != 0)
    {
      potential_replacements[n++] = i;
    }
  }
  if(n == 0)
  {
    // Can't change yet
    return;
  }
  int index = rand() % n;
  curr_pawn->type = potential_replacements[index];
}

int tile_empty(POS pos)
{
  // In bounds check
  if(out_of_bounds(pos))
  {
    return 0;
  }
  return get_tile(pos)->piece == NULL;
}

int tile_occed_by_piece(POS pos, int is_white)
{
  // In bounds check
  if(out_of_bounds(pos))
  {
    return 0;
  }
  if(is_white != 0)
  {
    is_white = 1;
  }
  return get_tile(pos)->piece != NULL && get_tile(pos)->piece->is_white == is_white;
}

void attack_tile(POS pos, int is_white)
{
  TILE* t = get_tile(pos);
  if(t->under_attack == 'n')
  {
    if(is_white)
    {
      t->under_attack = 'w';
    }else
    {
      t->under_attack = 'b';
    }
  }else if((t->under_attack == 'b' && is_white) || (t->under_attack == 'w' && !is_white))
  {
    t->under_attack = 'a';
  }
}

void reset_attacks()
{
  for (int i = 0; i < BOARD_SIZE; i++)
  {
    for (int j = 0; j < BOARD_SIZE; j++)
    {
      board[i][j].under_attack = 'n';
    }
  }
}

void recalculate_attacks(int is_white)
{
  reset_attacks();
  // Get opposite team's moves but dump the results
  // This is for marking under_attack flags
  int n;
  POSSIBLEMOVES* arr = get_all_team_moves(&n, is_white);
  // Free all allocated memory
  free_pointer(&arr, n);
}

int is_attacked_by(POS pos, int is_white)
{
  char att_status = get_tile(pos)->under_attack;
  if(att_status == 'a')
  {
    return 1;
  }
  return (att_status == 'w' && is_white) || (att_status == 'b' && !is_white);
}

TILE* get_tile(POS pos)
{
  return &board[pos.i][pos.j];
}

int out_of_bounds(POS pos)
{
  return pos.i < 0 || pos.i >= BOARD_SIZE || pos.j < 0 || pos.j >= BOARD_SIZE;
}

void free_pointer(POSSIBLEMOVES** pm, int n)
{
  for(int i = 0; i < n; i++)
  {
    free((*pm)[i].dest_arr);
  }
  free(*pm);
  *pm = NULL;
}

PIECE* create_piece(PIECE prototype)
{
  // Any other value besides 0 means it is a white piece
  if(prototype.is_white != 0)
  {
    prototype.is_white = 1;
  }
  PIECE* p = malloc(sizeof(PIECE));
  if(p == NULL)
  {
    printf("Not enough memory!\n");
    exit(1);
  }
  *p = prototype;
  // Set the piece pointer of the tile as well
  p->tile->piece = p;
  return p;
}

void remove_potential_move(POSSIBLEMOVES* pm, int index)
{
  if(pm->dest_n <= index)
  {
    return;
  }
  pm->dest_n--;
  for(int i = index; i < pm->dest_n; i++)
  {
    pm->dest_arr[i] = pm->dest_arr[i + 1];
  }
}

void initialize_board()
{
  for (int i = 0; i < BOARD_SIZE; i++)
  {
    for (int j = 0; j < BOARD_SIZE; j++)
    {
      board[i][j].pos = (POS){i, j};
      board[i][j].piece = NULL;
      board[i][j].under_attack = 'n';
    }
  }
  // Where the pawns are
  int i_white = 1;
  int i_black = 6;
  for(int j = 0; j < BOARD_SIZE; j++)
  {
    create_piece((PIECE){&board[i_white][j], Pawn, 1});
    create_piece((PIECE){&board[i_black][j], Pawn, 0});
  }
  // For convenience, keep the indexes of rows
  i_white = 0;
  i_black = 7;
  // Rooks
  create_piece((PIECE){&board[i_white][0], Rook, 1});
  create_piece((PIECE){&board[i_black][0], Rook, 0});
  create_piece((PIECE){&board[i_white][7], Rook, 1});
  create_piece((PIECE){&board[i_black][7], Rook, 0});
  // Knights
  create_piece((PIECE){&board[i_white][1], Knight, 1});
  create_piece((PIECE){&board[i_black][1], Knight, 0});
  create_piece((PIECE){&board[i_white][6], Knight, 1});
  create_piece((PIECE){&board[i_black][6], Knight, 0});
  // Bishops
  create_piece((PIECE){&board[i_white][2], Bishop, 1});
  create_piece((PIECE){&board[i_black][2], Bishop, 0});
  create_piece((PIECE){&board[i_white][5], Bishop, 1});
  create_piece((PIECE){&board[i_black][5], Bishop, 0});
  // Queens
  create_piece((PIECE){&board[i_white][3], Queen, 1});
  create_piece((PIECE){&board[i_black][3], Queen, 0});
  // Kings
  create_piece((PIECE){&board[i_white][4], King, 1});
  create_piece((PIECE){&board[i_black][4], King, 0});
}

void print_board()
{
  printf("\n");
  for (int i = BOARD_SIZE - 1; i >= 0; i--)
  {
    printf(" ");
    for(int j = 0; j < BOARD_SIZE; j++)
    {
      printf("┼───");
    }
    printf("┼\n");
    for (int j = 0; j < BOARD_SIZE; j++)
    {
      printf(" │ ");
      if(board[i][j].piece == NULL)
      {
        printf(" ");
      }else
      {
        if(!board[i][j].piece->is_white)
        {
          // Show black pieces as blue
          printf("\033[36m");
        }else
        {
          // Show white pieces as yellow
          printf("\033[33m");
        }
        switch(board[i][j].piece->type)
        {
          case Pawn:
            printf("♟");
            break;
          case Rook:
            printf("♜");
            break;
          case Knight:
            printf("♞");
            break;
          case Bishop:
            printf("♝");
            break;
          case Queen:
            printf("♛");
            break;
          case King:
            printf("♚");
            break;
        }
        // Reset
        printf("\033[0m");
      }
    }
    printf(" │\n");
  }
  printf(" ");
  for(int j = 0; j < BOARD_SIZE; j++)
  {
    printf("┼───");
  }
  printf("┼\n");
}
