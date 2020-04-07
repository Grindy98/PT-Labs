#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "huffman.h"

void exit_error_invalid_file()
{
  printf("Invalid file format to decode!\n");
  exit(1);
}

unsigned char read_header(FILE* fin, DATA_FREQ data[])
{
  unsigned char n;
  if(fread(&n, 1, 1, fin) != 1)
  {
    exit_error_invalid_file();
  }
  // Real value of how many data_freq pairs there are to read
  for(int i = 0; i <= n; i++)
  {
    if(fread(&data[i].data, 1, 1, fin) != 1)
    {
      exit_error_invalid_file();
    }
    if(fread(&data[i].weight, 1, 1, fin) != 1)
    {
      exit_error_invalid_file();
    }
  }
  return n;
}



int main(int argc, char* argv[])
{
  if(argc != 3)
  {
    printf("Command line arguments as: huff_dec [file to decode] [output file]\n");
    exit(1);
  }
  FILE* fin  = fopen(argv[1], "rb");
  FILE* fout = fopen(argv[2], "wb");
  if(fin == NULL)
  {
    printf("Error opening file to decode!\n");
    exit(1);
  }
  if(fout == NULL)
  {
    printf("Error opening output file!\n");
    exit(1);
  }
  // Read the header to get frequency information
  DATA_FREQ freq[SIZE_BYTE];
  unsigned char n = read_header(fin, freq);
  // Reconstruct the huffman tree
  HN* ht = get_huff_tree(freq, (unsigned) n);

  // We will iterate through the huffman tree
  // Use 0 as left and 1 as right
  HN* curr_node = ht;
  // We read the next 2 bytes in order to respect the last bits padding convention
  unsigned char curr_byte[2];
  if(fread(&curr_byte[0], 1, 1, fin) != 1)
  {
    // There has to be a first byte
    exit_error_invalid_file();
  }
  // Keep reading until the end of file
  while(fread(&curr_byte[1], 1, 1, fin) == 1)
  {
    for(int i = 0; i < 8; i++)
    {
      // Go bit by bit through the tree
      curr_node = curr_byte[0] & 0x80u >> i ? curr_node->right : curr_node->left;
      // Check if leaf node
      if(curr_node->left == NULL)
      {
        // Write the found bit to output
        fwrite(&curr_node->data, 1, 1, fout);
        // Reset node to root
        curr_node = ht;
      }
    }
    // Move over the next byte
    curr_byte[0] = curr_byte[1];
  }
  // Last bit to be handled following the padded 1 convention
  int bits_left = 7;
  while((curr_byte[0] & 0x80u >> bits_left) == 0 && bits_left != 0)
  {
    bits_left--;
  }
  for(int i = 0; i < bits_left; i++)
  {
    // Go bit by bit through the tree
    curr_node = curr_byte[0] & 0x80u >> i ? curr_node->right : curr_node->left;
    // Check if leaf node
    if(curr_node->left == NULL)
    {
      // Write the found bit to output
      fwrite(&curr_node->data, 1, 1, fout);
      // Reset node to root
      curr_node = ht;
    }
  }
  if(curr_node != ht)
  {
    printf("Decoding unsuccessful due to unfinished last byte!\n");
    exit(1);
  }

  remove_huff_tree(&ht);
  fclose(fin);
  fclose(fout);
  printf("File decoded successfully!\n");
  return 0;
}
