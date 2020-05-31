#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

typedef struct FILE_HEADER
{
  char name[100];
  // Store it as hex in characters
  char size[12];
} FILE_HEADER;

typedef struct FILE_STRUCT
{
  FILE_HEADER header;
  FILE* file;
} FILE_STRUCT;

void fill_header(FILE_HEADER* fh, FILE* f, char* f_name)
{
  strcpy(fh->name, f_name);
  fseek(f, 0L, SEEK_END);
  size_t size = ftell(f);
  rewind(f);
  snprintf(fh->size, 12, "%011lx", size);
}

void copy_file(FILE* dest, FILE* src, size_t size)
{
  char buf[512];
  for(int i = 0; i < size/512; i++)
  {
    fread(buf, 512, 1, src);
    fwrite(buf, 512, 1, dest);
  }
  // Write the last part
  fread(buf, size % 512, 1, src);
  fwrite(buf, size % 512, 1, dest);
}

int compare_files(const void* a1, const void* a2)
{
  const FILE_STRUCT * f1 = a1,* f2 = a2;
  return strcmp(f1->header.name, f2->header.name);
}

int compare_strings(const void* a1, const void* a2)
{
  char *const* f1 = a1;
  char *const* f2 = a2;
  return strcmp(*f1, *f2);
}


int main(int argc, char* args[])
{
  // -1 is error, 0 is archive, 1 is extract, 2 is list
  int mode = -1;
  if(argc >= 3)
  {
    if(strcmp("-a", args[1]) == 0)
    {
      mode = 0;
    }else if(strcmp("-x", args[1]) == 0)
    {
      mode = 1;
    }else if(strcmp("-l", args[1]) == 0)
    {
      mode = 2;
    }
  }
  if(mode == -1)
  {
    fprintf(stderr, "Usage: for archivation: ");
    fprintf(stderr, "for archivation:%s -a [ARCH NAME] [FILE NAMES...]/[Empty for all files]\n", args[0]);
    fprintf(stderr, "for extraction: %s -x [ARCH NAME] [FILE NAMES...]/[empty for all files]\n", args[0]);
    fprintf(stderr, "for listing: %s -l [ARCH NAME]\n", args[0]);
    exit(1);
  }
  if(mode == 0)
  {
    // Archive
    FILE * arch = fopen(args[2], "wb");
    if(arch == NULL)
    {
      fprintf(stderr, "Error opening the archive file %s\n", args[2]);
      exit(1);
    }

    FILE_STRUCT infiles[255];
    unsigned char n_files;
    // If no input files given -- archive all
    if(argc == 3)
    {
      n_files = 0;
      DIR *d;
      struct dirent *dir;
      d = opendir("./");
      if(d != NULL)
      {
        while ((dir = readdir(d)) != NULL)
        {
          // Add only files excepting the archive and the program itself
          if(dir->d_type == DT_REG && strcmp(args[2], dir->d_name) != 0 &&
            strcmp(args[0] + 2, dir->d_name) != 0)
          {
            infiles[n_files].file = fopen(dir->d_name, "rb");
            if(infiles[n_files].file == NULL)
            {
              fprintf(stderr, "Error opening the input file %s\n", dir->d_name);
              exit(1);
            }
            fill_header(&infiles[n_files].header, infiles[n_files].file, dir->d_name);
            n_files++;
          }
        }
        closedir(d);
      }
    }else
    {
      // Files given as params
      n_files = argc - 3;
      for(int i = 3; i < argc; i++)
      {
        infiles[i - 3].file = fopen(args[i], "rb");
        if(infiles[i - 3].file == NULL)
        {
          fprintf(stderr, "Error opening the input file %s\n", args[i]);
          exit(1);
        }
        fill_header(&infiles[i - 3].header, infiles[i - 3].file, args[i]);
      }
    }
    // Sort the files alphabetically before archiving for finding them easier during extraction
    qsort(infiles, n_files, sizeof(FILE_STRUCT), compare_files);
    // Starting our output arch with a 1 bit encoding of how many files we have (max 255)
    fwrite(&n_files, 1, 1, arch);
    // Printing the rest of the buffer
    for(int i = 0; i < n_files; i++)
    {
      fwrite(&infiles[i].header, sizeof(FILE_HEADER), 1, arch);
    }
    // Write each file with no space in between
    for(int i = 0; i < n_files; i++)
    {
      unsigned long size = 0;
      sscanf(infiles[i].header.size, "%lx", &size);
      copy_file(arch, infiles[i].file, size);
      // Close the file
      fclose(infiles[i].file);
    }
    fclose(arch);
  }else
  {
    // Extract or List
    FILE * arch = fopen(args[2], "rb");
    if(arch == NULL)
    {
      fprintf(stderr, "Error opening the archive file %s\n", args[2]);
      exit(1);
    }
    // Find the number of files in arch
    unsigned char n_files = 0;
    fread(&n_files, 1, 1, arch);
    // Reconstruct the headers
    FILE_HEADER headers[n_files];
    fread(headers, sizeof(FILE_HEADER), n_files, arch);
    if(mode == 2)
    {
      // Just print the names of the file and their sizes
      printf("There are %u files in archive %s:\n", n_files, args[2]);
      for(int i = 0; i < n_files; i++)
      {
        size_t curr_size = 0;
        sscanf(headers[i].size, "%lx", &curr_size);
        printf("File %s of size %lu bytes (0x%lx)\n", headers[i].name, curr_size, curr_size);
      }
    }else
    {
      // Extract files
      FILE* curr_outfile;
      size_t curr_size = 0;
      if(argc == 3)
      {
        // If there are no requested extract files -- extract all
        for(int i = 0; i < n_files; i++)
        {
          curr_outfile = fopen(headers[i].name, "wb");
          if(curr_outfile == NULL)
          {
            fprintf(stderr, "Error opening the extracted file %s\n", headers[i].name);
            exit(1);
          }
          sscanf(headers[i].size, "%lx", &curr_size);
          copy_file(curr_outfile, arch, curr_size);
          fclose(curr_outfile);
        }
      }else
      {
        char* files_to_extr[argc - 3];
        for(int i = 3; i < argc; i++)
        {
          // Place pointer to function names
          files_to_extr[i - 3] = args[i];
        }
        // Order said names alphabetically
        qsort(files_to_extr, argc - 3, sizeof(char*), compare_strings);

        // Iterate through the headers to check which files should be extracted
        // If not present in the headers => print error (file not found)
        // If not present in the files to extract => replace with null ptr
        int i = 0, j = 0;
        while(1)
        {
          if(i == argc - 3)
          {
            // All files left in the archive will NOT be extracted
            break;
          }
          if(j == n_files)
          {
            // All files to be extracted do not appear in the archive
            while(i != argc - 3)
            {
              fprintf(stderr, "File %s not found in archive!\n", files_to_extr[i]);
              i++;
            }
            break;
          }
          if(strcmp(files_to_extr[i], headers[j].name) == 0)
          {
            // Match, we want to extract this file
            FILE* curr_outfile = fopen(files_to_extr[i], "wb");
            if(curr_outfile == NULL)
            {
              fprintf(stderr, "Error opening the extracted file %s\n", headers[i].name);
              exit(1);
            }
            size_t size = 0;
            sscanf(headers[j].size, "%lx", &size);
            copy_file(curr_outfile, arch, size);
            fclose(curr_outfile);
            i++;
            j++;
          }else if(strcmp(files_to_extr[i], headers[j].name) > 0)
          {
            // File not requested, jump over this file
            size_t size = 0;
            sscanf(headers[j].size, "%lx", &size);
            // Move cursor at the start of the next file
            fseek(arch, size, SEEK_CUR);
            j++;
          }else
          {
            //Print an error and move along
            fprintf(stderr, "File %s not found in archive!\n", files_to_extr[i]);
            i++;
          }
        }
      }
    }
    fclose(arch);
  }
  return 0;
}
