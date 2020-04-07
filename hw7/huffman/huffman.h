#ifndef _HUFFMAN_H
#define _HUFFMAN_H

#define SIZE_BYTE 256

typedef struct HN
{
  unsigned weight;
  unsigned char data;
  struct HN* parent;
  struct HN* left,* right;
} HN;

typedef struct
{
  unsigned char data;
  unsigned char weight;
} DATA_FREQ;

int get_tree_height(HN* tree);
void get_leaves(HN* tree, HN* leaves[]);
HN* get_huff_tree(DATA_FREQ data[], unsigned n);
void remove_huff_tree(HN** tree);

void print_bits(unsigned char arr[], int n_bits);
void recursprint(HN* a);

#endif /* end of include guard: _HUFFMAN_H */
