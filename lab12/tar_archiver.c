#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

// 512 bytes
typedef struct HEADER
{
  char name[100];               //   0
  char mode[8];                 // 100
  char uid[8];                  // 108
  char gid[8];                  // 116
  char size[12];                // 124
  char mtime[12];               // 136
  char chksum[8];               // 148
  char typeflag;                // 156
  char linkname[100];           // 157
  char zeros[255];              // 257
}HEADER;

// Return file size to avoid calling stat again in main
long unsigned print_header(FILE* arch, char* name, FILE* input_file)
{
  HEADER h;
  // Init with 0
  memset(&h, 0x0, 512);
  // Write the name of the file
  if(strlen(name) > 99)
  {
    fprintf(stderr, "File %s has a name that is too long!\n", name);
    exit(1);
  }
  strcpy(h.name, name);

  struct stat file_stat;
  stat(name, &file_stat);

  // Write file mode according to stat
  snprintf(h.mode, 8, "%07o", file_stat.st_mode);
  printf("help %07o\n", file_stat.st_mode);
  // Write file uid according to stat
  snprintf(h.uid, 8, "%07o", file_stat.st_uid);
  // Write file gid according to stat
  snprintf(h.gid, 8, "%07o", file_stat.st_gid);
  // Write file size according to stat
  snprintf(h.size, 12, "%011lo", file_stat.st_size);
  // Write file mtime according to stat
  snprintf(h.mtime, 12, "%011lo", file_stat.st_mtime);
  // Write file typeflag to 0
  h.typeflag = '0';
  // Calculate the checksum -- 6 digits of octal
  unsigned checksum = 0;
  memset(h.chksum, ' ', 8);
  char* chks_p = (char*)&h;
  for(int i = 0; i < 512; i++)
  {
    checksum += (unsigned)*(chks_p + i);
  }
  // Correct format is 6 digits of checksum, followed by '\0' and ' '
  snprintf(h.chksum, 7, "%06o", checksum);
  fwrite(&h, 1, 512, arch);
  return file_stat.st_size;
}

int main(int argc, char* args[])
{
  if(argc < 3)
  {
    fprintf(stderr, "Usage:%s [TAR ARCH NAME] [FILE NAMES...]\n", args[0]);
    exit(1);
  }
  FILE * arch = fopen(args[1], "wb");
  if(arch == NULL)
  {
    fprintf(stderr, "Error opening the tar archive file %s\n", args[1]);
    exit(1);
  }
  FILE* infiles[argc - 2];
  for(int i = 2; i < argc; i++)
  {
    infiles[i - 2] = fopen(args[i], "rb");
    if(infiles[i - 2] == NULL)
    {
      fprintf(stderr, "Error opening the input file %s\n", args[i]);
      exit(1);
    }
  }
  // Go through all the files and print the header and contents
  for(int i = 2; i < argc; i++)
  {
    unsigned long size = print_header(arch, args[i], infiles[i - 2]);
    char buf[512];
    while(size > 512)
    {
      fread(buf, 1, 512, infiles[i - 2]);
      fwrite(buf, 1, 512, arch);
      size -= 512;
    }
    // Copy the last bytes in a 0 filled buf
    memset(buf, 0x0, 512);
    fread(buf, 1, size, infiles[i - 2]);
    fwrite(buf, 1, 512, arch);
    printf("Encoded file %s\n", args[i]);
  }
  fclose(arch);
  for(int i = 2; i < argc; i++)
  {
    fclose(infiles[i - 2]);
  }
  return 0;
}
