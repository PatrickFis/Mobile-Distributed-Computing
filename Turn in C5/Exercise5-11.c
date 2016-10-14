/* Exercise 5-11 is to write a program to calculate the sum of a simple harmonic progression.
 * The program specified that the user should be able to specify some arbitrary degree of precision.
 * I tried to use the GMP MP library for arbitrary precision decimal numbers, but it just introduced so many
 * problems with the program that I re-wrote it using the long double type in C.
 * The program can be compiled by typing
 * mpicc -o Exercise5-11 Exercise5-11.c -lm.
 */

#include <mpi.h>
#include <math.h>
#include <stdio.h>
#include "MyMPI.h"

int main(int argc, char *argv[]) {
  double elapsed_time; // Parallel execution time
  long double i;
  int id; // Process ID number
  int p; // Number of processes
  int n; // Range of series
  int d; // Precision of series
  long double global_sum; // Results from each process will be summed into this.
  global_sum = 0;
  MPI_Init(&argc, &argv);

  /* Start the timer */
  MPI_Barrier(MPI_COMM_WORLD);
  elapsed_time = -MPI_Wtime();
  MPI_Comm_rank(MPI_COMM_WORLD, &id);
  MPI_Comm_size(MPI_COMM_WORLD, &p);

  if(id == 0) { // Get n and d, broadcast to other processes.
    printf("Enter n: ");
    scanf("%d", &n);
    printf("Enter d: ");
    scanf("%d", &d);
  }
  MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&d, 1, MPI_INT, 0, MPI_COMM_WORLD);

  int step[p+1]; // This will hold the lower and upper bounds for the for loop for each process.
  int stepSize = n/p;
  for(int j = 0; j < p; j++) {
    step[j] = j*stepSize;
  }
  step[0] = 1;
  step[p] = n+1;
  // for(int j = 0; j < p+1; j++) {
  //   printf("%d, id: %d\n", step[j], id);
  // }
  long double sum = 0.0; // Local sum for each process

  for(i = step[id]; i < step[id+1]; i++) {
    long double frac = (long double)(1/i);
    sum += frac;
    // printf("%Lf\n",sum);
  }
  MPI_Reduce(&sum, &global_sum, 1, MPI_LONG_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
  if(id == 0) {

    printf("sum = %.*Lf\n", d, global_sum);
  }
  MPI_Finalize();
  return 0;
}
