#include <stdio.h>
#include <stdlib.h>

typedef struct
{
  int cost;
  int dest_ind;
} ROAD;

int compare_roads(const void* a1, const void* a2)
{
  return ((ROAD*)a1)->cost - ((ROAD*)a2)->cost;
}

int greedy(int node, int visited_nodes[], int n, int starting_node, int adj[n][n])
{
  visited_nodes[node] = 1;
  // Check base exit condition -> found path
  int all_nodes_visited = 1;
  for(int i = 0; i < n; i++)
  {
    if(visited_nodes[i] == 0)
    {
      all_nodes_visited = 0;
    }
  }
  if(all_nodes_visited && adj[node][starting_node] != -1)
  {
    // If you visited all nodes and there is a path to the beginning
    printf("%d<-%d", starting_node, node);
    return adj[node][starting_node];
  }
  ROAD road_arr[n];
  for(int j = 0; j < n; j++)
  {
    road_arr[j] = (ROAD){adj[node][j], j};
  }
  qsort(road_arr, n, sizeof(ROAD), compare_roads);
  // Sort the road array to get the min cost and ignore -1

  for(int i = 0; i < n; i++)
  {
    if(road_arr[i].cost != -1)
    {
      if(visited_nodes[road_arr[i].dest_ind] == 0)
      {
        // There is an unvisited node
        int cost = greedy(road_arr[i].dest_ind, visited_nodes, n, starting_node, adj);
        if(cost != -1)
        {
          // We found the path
          printf("<-%d", node);
          return cost + road_arr[i].cost;
        }
      }
    }
  }
  // If we reached this point -> no path is valid
  visited_nodes[node] = 0;
  return -1;
}

int main(int argc, char* argv[])
{
  if(argc != 2)
  {
    printf("Add the file name containing the adjacency matrix as a command line argument!\n");
    return 1;
  }
  FILE* fin = fopen(argv[1], "r");
  if(fin == NULL)
  {
    printf("Given file does not exist!\n");
    return 1;
  }
  int n = -1;
  fscanf(fin, "%d", &n);
  if(n <= 0)
  {
    printf("File should have on the first line the size of the matrix!\n");
    return 1;
  }
  int adj[n][n];
  for(int i = 0; i < n; i++)
  {
    for(int j = 0; j < n; j++)
    {
      if(fscanf(fin, "%d", &adj[i][j]) != 1)
      {
        printf("Matrix format is invalid!\n");
        return 1;
      }
      if(adj[i][j] <= 0)
      {
        // Not a road, set to -1
        adj[i][j] = -1;
      }
    }
  }
  // Properly read the adj matrix
  int visited_nodes[n];
  for(int i = 0; i < n; i++)
  {
    visited_nodes[i] = 0;
  }
  int cost = greedy(0, visited_nodes, n, 0, adj);
  if(cost == -1)
  {
    printf("No valid path found!\n");
  }else
  {
    printf("\nFinal path costs %d\n", cost);
  }
  return 0;
}
