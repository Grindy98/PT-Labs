#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "huffman.h"



// Below functions only used locally in get_huff_tree
int compare_HN(const void* a1, const void* a2)
{
  return -( (*(HN**)a1)->weight - (*(HN**)a2)->weight );
}
HN* get_huff_tree(DATA_FREQ data[], unsigned n)
{
  // n from (0, 255) to (1, 256)
  n++;
  HN* arr[n];
  for(int i = 0; i < n; i++)
  {
    // Initialize the array(all leaf HN's) which will become the huffman tree
    arr[i] = malloc(sizeof(HN));
    if(arr[i] == NULL)
    {
      printf("Not enough memory!\n");
      exit(1);
    }
    arr[i]->data = data[i].data;
    arr[i]->weight = data[i].weight;
    arr[i]->left = arr[i]->right = NULL;
    arr[i]->parent = NULL;
  }
  // Repeat until we have only one node -> the root
  while(n > 1)
  {
    // Sort descending each time
    qsort(arr, n, sizeof(HN*), compare_HN);
    // Pick last two elements and form a new tree
    HN* last = arr[n - 1];
    HN* second = arr[n - 2];
    // Remove the 2 elems from the array and add the new huffman tree
    HN* new_node = malloc(sizeof(HN));
    if(new_node == NULL)
    {
      printf("Not enough memory!\n");
      exit(1);
    }
    n--;
    new_node->data = 0;
    new_node->right = last;
    new_node->left = second;
    new_node->weight = last->weight + second->weight;
    // Set parent dependency
    last->parent = second->parent = new_node;
    // Insert new node back in array
    arr[n - 1] = new_node;
  }

  return arr[0];
}

int get_tree_height(HN* tree)
{
  if(tree == NULL)
  {
    return 0;
  }
  int left = get_tree_height(tree->left);
  int right = get_tree_height(tree->right);
  return 1 + (left > right ? left : right);
}

void get_leaves(HN* tree, HN* leaves[])
{
  if(tree->left == NULL)
  {
    // It is a leaf node
    leaves[tree->data] = tree;
    return;
  }
  get_leaves(tree->left , leaves);
  get_leaves(tree->right, leaves);
}

void remove_huff_tree(HN** tree)
{
  if((*tree)->left != NULL)
  {
    remove_huff_tree(&((*tree)->left));
  }
  if((*tree)->right != NULL)
  {
    remove_huff_tree(&((*tree)->right));
  }
  free(*tree);
  *tree = NULL;
}


void print_bits(unsigned char arr[], int n_bits)
{
  for(int i = 0; i < n_bits; i++)
  {
    arr[i / 8] & (0x80u >> i % 8) ? putchar('1') : putchar('0');
  }
}

void recursprint(HN* a)
{
  static char string[SIZE_BYTE] = "";
  if(a->left == NULL)
  {
    printf("Encoding for char %c:%s\n",a->data, string);
    return;
  }
  int l = strlen(string);
  strcat(string, "0");
  recursprint(a->left);
  string[l] = '\0';
  strcat(string, "1");
  recursprint(a->right);
  string[l] = '\0';
}
