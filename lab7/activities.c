#include <stdio.h>
#include <stdlib.h>

typedef struct
{
  unsigned int start_hour:5;
  unsigned int start_min:6;
  unsigned int end_hour:5;
  unsigned int end_min:6;
} ACTIVITY;

int sort_by_end(const void* a1, const void* a2)
{
  ACTIVITY b1 = *(ACTIVITY*)a1;
  ACTIVITY b2 = *(ACTIVITY*)a2;
  if(b1.end_hour != b2.end_hour)
  {
    return b1.end_hour - b2.end_hour;
  }
  return b1.end_min - b2.end_min;
}

int is_starting_after(ACTIVITY a1, ACTIVITY a2)
{
  return (a1.end_hour < a2.start_hour) ||
  (a1.end_hour == a2.start_hour && a1.end_min <= a2.start_min);
}

int main()
{
  ACTIVITY arr[] = {{3,26 , 5,33}, {4,15 , 4,30}, {6,11 , 7,12}, {2,14 , 5,32}, {4,31 , 6,11}, {6,30 , 7,0}, {1,20 , 5,22}};
  int n = sizeof(arr)/sizeof(arr[0]);

  if(n == 0)
  {
    printf("There are no activities available!\n");
    return 0;
  }

  qsort(arr, n, sizeof(ACTIVITY), sort_by_end);

  int max_activities = 1;
  int last_choice_index = 0;
  int found_new_choice = 1;
  while(found_new_choice)
  {
    printf("Activity from %d:%02d to %d:%02d\n", arr[last_choice_index].start_hour,
    arr[last_choice_index].start_min, arr[last_choice_index].end_hour, arr[last_choice_index].end_min);
    for(int i = last_choice_index; i < n; i++)
    {
      if(is_starting_after(arr[last_choice_index], arr[i]))
      {
        // Found the next best activity choice
        last_choice_index = i;
        max_activities++;
        found_new_choice = 1;
        break;
      }else
      {
        found_new_choice = 0;
      }
    }
  }
  printf("There are a maximum of %d activities that fit into the schedule.\n", max_activities);
  return 0;
}
