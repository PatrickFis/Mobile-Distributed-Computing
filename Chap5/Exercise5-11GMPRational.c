/*
* Exercise 5-11 is to write a program to calculate the sum of a simple harmonic progression.
* Since this assignment requires the use of arbitrary precision, I decided to use the GMP MP Bignum
* library to save some time. The program can be compiled by typing
* mpicc -o Exercise5-11 Exercise5-11GMP.c -lgmp. The last flag is to link the GMP library.
*
* mpq_t is a rational number
* mpq_init(var) = set to 0/1
* mpq_canonicalize(var) eliminate common factors between numerator and denominator of var
*/

#include <mpi.h>
#include <math.h>
#include <stdio.h>
#include <gmp.h>
#include "MyMPI.h"

int main(int argc, char *argv[]) {
  double elapsed_time; // Parallel execution time
  int i;
  int id; // Process ID number
  int p; // Number of processes
  int n; // Range of series
  int d; // Precision of series
  mpq_t global_sum; // Holds final sum
  mpq_init(global_sum);
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
  // printf("%d %d %d\n", n,d,id);

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
  mpq_t sum; // Local sum
  mpq_init(sum);
  mpq_t frac; // Fraction used in for loop
  mpq_init(frac);
  for(i = step[id]; i < step[id+1]; i++) {
    mpq_set_si(frac, 1, i); // frac = 1/i
    // mpq_canonicalize(frac);
    // gmp_printf("frac=%F\n", frac);
    mpq_add(sum, sum, frac);
    // gmp_printf("sum=%F\n");
  }
  MPI_Finalize();
  return 0;
}
