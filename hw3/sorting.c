#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "sorting.h"

// Max size and default size of array
#define N_MAX 100000
#define N_DEFAULT 10000

unsigned long assig_count_merge_sort = 0;
unsigned long swap_count_inser_sort = 0;

unsigned long compar_count_bin_search = 0;
unsigned long compar_count_lin_search = 0;

int main()
{
    // Seed the rand function
    srand(time(NULL));

    unsigned long N = 1;
    int exp = 0;
    printf("Set the size of random array(exp of 2):");

    if(scanf("%d", &exp) != 1 || exp < 0)
    {
        printf("Input error - Size set to %d.\n", N_DEFAULT);
        N = N_DEFAULT;
    }else
    {
        for(int i = 0; i < exp; i++)
        {
            N <<= 1;
            if(N > N_MAX)
            {
                printf("Exponent too large - Size set to %d.\n", N_MAX);
                N = N_MAX;
                break;
            }
        }
    }

    int v_orig[N], v_merge[N], v_inser[N];

    printf("Populating the arrays with %lu random values.\n", N);
    for(int i = 0; i < N; i++)
    {
        v_orig[i] = v_merge[i] = v_inser[i] = rand()%100000;
    }
    printf("\033[1m\033[33m\n  SORTING:\n\033[0m");
    
    // REDO INSER SORT CUZ NOW IT IS SELECTION SORT
    inser_sort(v_inser, N);
    merge_sort(v_merge, N);

    int ok = 1;
    for(int i = 0; i < N; i++)
    {
        //printf("%d  %d\n", v_inser[i], v_merge[i]);
        if(v_merge[i] != v_inser[i])
        {
            ok = 0;
        }
    }
    printf("%s\n", ok ? "Sorts match!": "Sorts don't match!");
    printf("Swaps for insertion sort: %lu\n", swap_count_inser_sort);
    printf("Assignments for merge sort: %lu\n", 
    assig_count_merge_sort);
    
    printf("\033[1m\033[33m\n  SEARCHING:\n\033[0m");

    // Choose a random element in the original array
    int i_rand = rand() % N;
    printf("Randomly chosen number to be found: %d\n", v_orig[i_rand]);
    int i_bin = bin_search(v_orig[i_rand], v_inser, N);
    int i_lin = lin_search(v_orig[i_rand], v_orig, N);
    if(i_bin == -1 || i_lin == -1)
    {
        printf("ERROR - Element not found!\n");
        exit(1);
    }

    printf("Binary search found %d with %lu comparisons.\n", 
    v_inser[i_bin], compar_count_bin_search);
    printf("Linear search found %d with %lu comparisons.\n", 
    v_orig[i_lin], compar_count_lin_search);

    return 0;
}

void inser_sort(int arr[], int n)
{
    for(int i = 1; i < n; i++)
    {
        int j = i;
        while(j >= 1 && arr[j] < arr[j - 1])
        {
            // Out of order elements
            int temp = arr[j];
            arr[j] = arr[j-1];
            arr[j-1] = temp;
            swap_count_inser_sort++;
            j--;
        }
    }
}

void merge_sort(int arr[], int n)
{
    if(n == 1)
    {
        // Base case, for sure sorted (single element)
        return;
    }
    int n1 = n / 2;
    int *arr1 = arr;
    int n2 = n - n1;
    int *arr2 = arr + n1;

    // Sort the 2 split arrays
    merge_sort(arr1, n1);
    merge_sort(arr2, n2);

    // Merge the 2 arrays into a 3rd temp storage
    int final[n], final_size = 0;
    int i = 0, j = 0;
    while(1)
    {
        if(arr1[i] <= arr2[j])
        {
            // If they are equal, take the item from arr1 first
            final[final_size++] = arr1[i++];
        }else
        {
            final[final_size++] = arr2[j++];
        }

        // If one of the ends was reached, fill the final array
        // with the other array's elements
        if(i == n1)
        {
            for(; j < n2; j++)
            {
                final[final_size++] = arr2[j];
            }
            break;
        }
        if(j == n2)
        {
            for(; i < n1; i++)
            {
                final[final_size++] = arr1[i];
            }
            break;
        }
    }
    // Exactly n elements have been assigned to final
    // Plus another n if we take into account the memcpy
    assig_count_merge_sort += 2*n;

    // Putting the result back into the original array
    memcpy(arr, final, sizeof(int) * n);
}   

int bin_search(int find, int arr[], int n)
{
    // Sorted array
    int i = 0;
    int j = n - 1;
    while(i <= j)
    {
        int mid = (i + j)/2;
        // Two comparisons are made
        if(arr[mid] == find)
        {
            // Item found
            compar_count_bin_search++;
            return mid;
        }else if(arr[mid] > find)
        {
            // Item in first half
            j = mid - 1;
            compar_count_bin_search += 2;
        }else
        {
            // Item in second half
            i = mid + 1;
            compar_count_bin_search += 2;
        }
    }
    // Item not found in arr
    return -1;
}

int lin_search(int find, int arr[], int n)
{
    for(int i = 0; i < n; i++)
    {
        compar_count_lin_search++;
        if(arr[i] == find)
        {
            // Item found in arr
            return i;
        }
    }
    // Item not found in arr
    return -1;
}