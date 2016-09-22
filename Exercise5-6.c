/*
* I left in most of my debug comments and some of my notes. The main issue that I've noticed
* is that this program will not output the correct answer when it is run with more than 2 processes.
* The section that could be leading to this problem are on lines 98-143(not actually lots of code, mostly comments).
*/






/*
* Sieve of Eratosthenes
*/
// Compile with mpicc -o Exercise5-6 Exercise5-6.c -lm
//
// Program is double dipping with some numbers, like if I run it like this:
// mpirun -np 2 ./Exercise5-6 200, 101 will be checked by process 0 and 1
// Appears to be fixed by adding a check in the for loop responsible for checking which numbers are marked
// I will just assume that attempting to run the code with more processes allocated
// than are actually available is supposed to produce errors.

#include <mpi.h>
#include <math.h>
#include <stdio.h>
#include "MyMPI.h"
#define min(a,b) ((a<b)?(a):(b))

int main(int argc, char *argv[]) {
  int count; // Local prime count
  double elapsed_time; // Parallel execution time
  int first; // Index of first multiple
  int global_count; // Global prime count
  int high_value; // Highest value on this proc
  int i;
  int id; // Process ID number
  int index; // Index of current prime
  int low_value; // Lowest value on this proc
  char *marked; // Portion of 2, ... , 'n'
  int n; // Sieving fron 2, ... , 'n'
  int p; // Number of processes
  int proc0_size; // Size of proc 0's subarray
  int prime; // Current prime
  int size; // Elements in 'marked'

  MPI_Init(&argc, &argv);

  /* Start the timer */
  MPI_Barrier(MPI_COMM_WORLD);
  elapsed_time = -MPI_Wtime();
  MPI_Comm_rank(MPI_COMM_WORLD, &id);
  MPI_Comm_size(MPI_COMM_WORLD, &p);

  if(argc != 2) {
    if(!id) printf("Command line: %s <m>\n", argv[0]);
    MPI_Finalize();
    exit(1);
  }

  n = atoi(argv[1]);

  /* Figure out this process's share of the array, as well as the integers represented
  by the first and last array elements */

  low_value = 2 + BLOCK_LOW(id,p,n-1);
  if(low_value%2==0) low_value++;
  high_value = 2 + BLOCK_HIGH(id,p,n-1);
  if(high_value%2==0) high_value--;
  size = BLOCK_SIZE(id,p,n-1);
  if(size%2==0) size /= 2;
  else size = (size+1) / 2;
  // printf("low: %d, high: %d, size: %d, id: %d\n", low_value, high_value, size); // debug

    /* Bail out if all the primes used for sieving are not all held by process 0 */

    proc0_size = (n-1)/p;

    if((2 + proc0_size) < (int) sqrt((double)n)) {
      if(!id) printf("Too many processes\n");
      MPI_Finalize();
      exit(1);
    }

    /* Allocate this process's share of the array. */

    marked = (char *) malloc(size);

    if(marked == NULL) {
      printf("Cannot allocate enough memory\n");
      MPI_Finalize();
      exit(1);
    }

    // marked should look like [0,1,3,5,7,9,11...n]
    for(i = 0; i < size; i++) marked[i] = 0;
    if(!id) index = 0;
    prime = 3; // Start at 3, not 2
    do {
      // if(prime * prime > low_value) // Code from original sieve
      // first = prime * prime - low_value;
      // else {
      //   if(!(low_value % prime)) first = 0;
      //   else first = prime - (low_value % prime);
      // }
      if(prime*prime > low_value) {
        first = (prime*prime - low_value)/2; // Actual index should be half this value because the
        // array only has half the memory allocated to it.
      }
      else { // If prime*prime < low_value, then we aren't on process 0...
        // printf("prime*prime = %d, id=%d\n", prime*prime, id); // debug
        if(!(low_value % prime)) {
          first = 0;
          // printf("else if prime=%d\n", prime);
        }
        else {
          // Need to find the index of the first prime that needs to be marked
          // by the sieve. Perhaps this can be done with a modulus operation.
          // There must be something wrong with this block. The program
          // either outputs the correct answer or is off by one.
          // 9/22 - Fixed this... at least for 1 to 2 processes

          int tmp_index = prime - (low_value % prime);
          // printf("tmp_index = %d ", tmp_index); // debug
          for(i = 0; i < size; i++) {
            // printf("%d\n",i); // debug
            // int tmp = ((low_value) + i*p) % prime; // Not sure if I'm going to use this variable
            if((low_value + i*p) % prime == 0) {
              // printf("%d, i=%d, prime=%d\n",low_value+(i*p),i,prime); // debug
              tmp_index = i;
              break;
            }
          }
          first = tmp_index;
          // printf("first = %d, id = %d\n", first, id);
        }
      }

      for(i = first; i < size; i += prime) marked[i] = 1;

      if(!id) {
        while(marked[++index]);
        prime = 3 + (index * 2); // Primes start at 3, index must be multiplied by 2 to find "real" value
      }
      MPI_Bcast(&prime, 1, MPI_INT, 0, MPI_COMM_WORLD);
    } while(prime * prime <= n);

    count = 0;
    // printf("id: %d\n", id); // debug
    for(i = 0; i < size; i++) {
      if(!marked[i]) {
        if(low_value + i*(p) > high_value) continue; // should id+1 be p? Or vice versa?
        count++;
        // printf("%d, ", (low_value)+i*(p)); // debug
      }
    }
    // printf("\n, n=%d\n", n); // debug
    MPI_Reduce(&count, &global_count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    /* Stop the timer */
    elapsed_time += MPI_Wtime();


    /* Print the results */
    if(!id) {
      printf("%d primes are less than or equal to %d\n", global_count + 1, n);
      // global_count + 1 -> 2 is a prime that is not represented in the array
      printf("Total elapsed time: %10.6f\n", elapsed_time);
      // for(i = 0; i < n; i++) { // debug
      //   if(!marked[i]) printf("%d, ", i);
      // }
    }
    MPI_Finalize();
    // printf("\nid = %d\n", id);
    // for(i = 0; i < n; i++)
    //   if(!marked[i]) printf("%d, ", i);
    // printf("\n");
    return 0;
  }
