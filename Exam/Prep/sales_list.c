#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct RECORD
{
  char* region;
  char* country;
  double total_cost;
  double total_profit;
  struct RECORD* next;
} RECORD;

// For convenience when allocating new memory
void check_alloc(void* p)
{
  if(p == NULL)
  {
    fprintf(stderr, "Not enough memory!\n");
    exit(1);
  }
}

RECORD* read_record(FILE* csv)
{
  // Read next line from file
  char* line = NULL;
  size_t cap = 0;
  // Check if EOF reached
  if(getline(&line, &cap, csv) == EOF)
  {
    return NULL;
  }

  RECORD* new_rec = malloc(sizeof(RECORD));
  check_alloc(new_rec);

  // Preemptively fill the next pointer with NULL
  new_rec->next = NULL;
  check_alloc(new_rec);

  // Char * array to indicate each entry from the csv file (we know there are 14)
  char* entry[14];
  entry[0] = strtok(line, ",");
  for(int i = 1; i < 14; i++)
  {
    entry[i] = strtok(NULL, ",");
    if(entry[i] == NULL)
    {
      fprintf(stderr, "Incorrect format!\n");
      exit(1);
    }
  }
  // Store the right entries into the struct
  check_alloc(new_rec->region = strdup(entry[0]));
  check_alloc(new_rec->country = strdup(entry[1]));
  if(sscanf(entry[12], "%lf", &new_rec->total_cost) == 0)
  {
    fprintf(stderr, "Incorrect format!\n");
    exit(1);
  }
  if(sscanf(entry[13], "%lf", &new_rec->total_profit) == 0)
  {
    fprintf(stderr, "Incorrect format!\n");
    exit(1);
  }

  // Free allocated line memory
  free(line);

  return new_rec;
}

int compare(RECORD* a, RECORD* b)
{
  // Compare alphabetically with resp to region then country
  int diff = strcmp(a->region, b->region);
  if(diff == 0)
  {
    diff = strcmp(a->country, b->country);
  }
  return diff;
}

void add_to_list(RECORD** list, RECORD* new_rec)
{
  // Empty list
  if(*list == NULL)
  {
    *list = new_rec;
    return;
  }
  // Check if first list elem is after the new elem;
  if(compare(new_rec, *list) < 0)
  {
    new_rec->next = *list;
    *list = new_rec;
    return;
  }
  // Iterate the list until you find the first elem greater than new_rec
  RECORD* prev = *list;
  RECORD* curr = prev->next;
  while(curr != NULL)
  {
    if(compare(new_rec, curr) < 0)
    {
      prev->next = new_rec;
      new_rec->next = curr;
      return;
    }
    curr = curr->next;
    prev = prev->next;
  }
  // If no element greater than new_rec, append it in the last place
  prev->next = new_rec;
}

int main(int argc, char* args[])
{
  if(argc != 3)
  {
    // Print usage for user since they gave the wrong number of arguments
    fprintf(stderr, "Usage: %s [CSV File] [Output File]\n", args[0]);
    exit(1);
  }
  FILE* csv = fopen(args[1],"r");
  if(csv == NULL)
  {
    // Unable to open file
    fprintf(stderr, "Unable to open file %s!\n", args[1]);
    exit(1);
  }
  FILE* fout = fopen(args[2], "w");
  if(fout == NULL)
  {
    // Unable to open file
    fprintf(stderr, "Unable to open file %s!\n", args[2]);
    exit(1);
  }

  // Dump the first line of the csv file
  fscanf(csv, "%*[^\n]\n");

  RECORD* list = NULL;

  while(1)
  {
    RECORD* r = read_record(csv);
    if(r == NULL)
    {
      break;
    }
    // Add each read element into the sorted list
    add_to_list(&list, r);
  }
  // Iterate through the list, print and find max vals for total cost and profit
  RECORD* curr = list;
  double currprofit_region = 0.0;
  double maxprofit_region = 0.0;
  char* leading_region_name = NULL;
  double currprofit_country = 0.0;
  double maxprofit_country = 0.0;
  char* leading_country_name = NULL;
  while(curr != NULL)
  {
    // Add the current entry's profit to both region and country
    currprofit_region += curr->total_profit;
    currprofit_country+= curr->total_profit;
    // Check if region will change
    if(curr->next == NULL || strcmp(curr->next->region, curr->region) != 0)
    {
      // Update maxprofit_region accordingly
      if(currprofit_region > maxprofit_region)
      {
        printf("CHANGED MAXREGION %s - %lf to %s - %lf\n", leading_region_name, maxprofit_region, curr->region, currprofit_region);
        maxprofit_region = currprofit_region;
        leading_region_name = curr->region;
        // Reset
        currprofit_region = 0.0;
      }
    }
    // Check if country will change
    if(curr->next == NULL || strcmp(curr->next->country, curr->country) != 0)
    {
      // Update maxprofit_country accordingly
      if(currprofit_country > maxprofit_country)
      {
        printf("CHANGED MAXCOUNTRY %s - %lf to %s - %lf\n", leading_country_name, maxprofit_country, curr->country, currprofit_country);
        maxprofit_country = currprofit_country;
        leading_country_name = curr->country;
        // Reset
        currprofit_country = 0.0;
      }
    }
    fprintf(fout, "%s - %s - %lf - %lf\n", curr->region, curr->country, curr->total_cost, curr->total_profit);
    curr = curr->next;
  }
  fprintf(fout, "Most profitable region: %s\n", leading_region_name);
  fprintf(fout, "Most profitable country: %s\n", leading_country_name);
  return 0;
}
