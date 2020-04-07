#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "huffman.h"


int get_freq(FILE* fin, DATA_FREQ pair[])
{
  double raw_weight[SIZE_BYTE];
  for(int i = 0; i < SIZE_BYTE; i++)
  {
    raw_weight[i] = 0.0f;
  }

  unsigned char c;
  while(fread(&c, 1, 1, fin) == 1)
  {
    // Add 1 for each element;
    raw_weight[c] += 1.0;
  }
  // For more precision, search for the maximum element and scale the array to get 254.99
  double maximum = 0.0;
  for(int i = 0; i < SIZE_BYTE; i++)
  {
    if(raw_weight[i] > maximum)
    {
      maximum = raw_weight[i];
    }
  }
  // Assign the output arrays with the converted value
  int n = 0;
  for(unsigned i = 0; i < SIZE_BYTE; i++)
  {
    if(raw_weight[i] > 0.0)
    {
      // Byte exists
      pair[n].data = i;
      // With values between 1 and 255
      pair[n].weight = ceil(raw_weight[i] * 254.99/maximum);
      n++;
    }
  }
  // Reset the pointer to file
  rewind(fin);
  // n = -1 is a flag that there are no characters encoded
  // n = 0 -> 1 char, n = 255 -> 256 chars
  return n - 1;
}

void write_header_data(FILE* fout, DATA_FREQ data[], unsigned n)
{
  // Write first bit signaling how many chars are encoded
  if(fwrite(&n, 1, 1, fout) != 1)
  {
    printf("Write error!\n");
    exit(1);
  }
  for(int i = 0; i <= n; i++)
  {
    // Write each char encoded and its frequency in order to be able to reconstruct
    // the huffman tree when decoding
    if(fwrite(&data[i].data, 1, 1, fout) != 1)
    {
      printf("Write error!\n");
      exit(1);
    }else if(fwrite(&data[i].weight, 1, 1, fout) !=1)
    {
      printf("Write error!\n");
      exit(1);
    }
  }
}

void write_and_pop_byte(FILE* fout, unsigned char queue[], int* n_bits)
{
  if(*n_bits < 8)
  {
    printf("Error, attempted write of an incomplete byte!\n");
    exit(1);
  }
  fwrite(&queue[0], 1, 1, fout);
  // Remove first element
  // n_bits / 8 + 1 makes sure that the last byte is copied over by a 0 byte -- IMPORTANT FOR BYTE SET
  for(int i = 0; i < *n_bits / 8 + 1 ; i++)
  {
    queue[i] = queue[i + 1];
  }

  *n_bits -= 8;
}

int main(int argc, char* argv[])
{

  if(argc != 3)
  {
    printf("Command line arguments as: huff_enc [file to encode] [output file]\n");
    exit(1);
  }
  FILE* fin  = fopen(argv[1], "rb");
  FILE* fout = fopen(argv[2], "wb");
  if(fin == NULL)
  {
    printf("Error opening file to encode!\n");
    exit(1);
  }
  if(fout == NULL)
  {
    printf("Error opening output file!\n");
    exit(1);
  }
  DATA_FREQ freq[SIZE_BYTE];
  int n_freq = get_freq(fin, freq);
  if(n_freq <= -1)
  {
    printf("File is empty!\n");
    exit(0);
  }
  if(n_freq >= 256)
  {
    printf("Miscounted number of bytes to encode!\n");
    exit(1);
  }
  HN* ht = get_huff_tree(freq, (unsigned)n_freq);
  // Write the header first
  write_header_data(fout, freq, (unsigned)n_freq);

  // Get all character leaves; if null, not a leaf
  HN* leaves[SIZE_BYTE];
  memset(leaves, 0, SIZE_BYTE * sizeof(HN*));
  get_leaves(ht, leaves);

  int n_max_tree = get_tree_height(ht);
  // Find out what is the maximum byte SIZE_BYTE of an encoding
  // then add 1 to account for an uncompleted byte and another 2 to have a buffer
  unsigned char encoded_buf[3 + n_max_tree / 8];
  // Initialize the buffer with 0's - IMPORTANT FOR BIT SET
  memset(encoded_buf, 0, 3 + n_max_tree / 8);
  int curr_bit = 0;
  unsigned char byte_to_encode;

  // 0's and 1's represented as a char (temporary storage)
  unsigned char encoding_bits[n_max_tree];
  while(fread(&byte_to_encode, 1, 1, fin) == 1)
  {
    // Still reading bytes of data until EOF
    if(leaves[byte_to_encode] == NULL)
    {
      printf("ERROR - Byte %x not processed for encoding!\n", byte_to_encode);
      exit(1);
    }
    HN* curr_node = leaves[byte_to_encode];

    int n_enc = 0;
    // While we haven't reached the root add bits to encoding
    while(curr_node->parent != NULL)
    {
      if(curr_node->parent->left == curr_node)
      {
        // Left path means 0
        encoding_bits[n_enc] = 0;
      }else
      {
        // Right path means 1
        encoding_bits[n_enc] = 1;
      }
      n_enc++;
      curr_node = curr_node->parent;
    }

    // Read bits in reverse order and merge them into the buffer
    for(int i = n_enc - 1; i >= 0; i--)
    {
      if(encoding_bits[i])
      {
        // Set the corresponding bit
        encoded_buf[curr_bit/8] |= 0x80u >> curr_bit % 8;
      }
      // 0 stored by default
      curr_bit++;
    }
    // While more than a byte in buffer, write buffer to file
    while(curr_bit >= 8)
    {
      write_and_pop_byte(fout, encoded_buf, &curr_bit);
    }
  }
  // Pad the remaining buffer with a 1 following however many zeros are needed to fill a byte
  // That way, we know to ignore however many 0's until we reach the last 1 in memory when we decode
  encoded_buf[0] |= 0x80u >> curr_bit;
  fwrite(&encoded_buf[0], 1, 1, fout);
  remove_huff_tree(&ht);
  fclose(fin);
  fclose(fout);
  printf("File encoded successfully!\n");
  return 0;
}
