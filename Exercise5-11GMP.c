/*
* Exercise 5-11 is to write a program to calculate the sum of a simple harmonic progression.
* Since this assignment requires the use of arbitrary precision, I decided to use the GMP MP Bignum
* library to save some time. The program can be compiled by typing
* mpicc -o Exercise5-11 Exercise5-11GMP.c -lgmp. The last flag is to link the GMP library.
*
* mpf_t is a rational number
* mpf_init(var) = set to 0 with default precision
* mpf_init2(var, num) set to 0 with num bits of precision
*/

#include <mpi.h>
#include <math.h>
#include <stdio.h>
#include <gmp.h>
#include "MyMPI.h"

int main(int argc, char *argv[]) {
  double elapsed_time; // Parallel execution time
  double i;
  int id; // Process ID number
  int p; // Number of processes
  int n; // Range of series
  int d; // Precision of series
  mpf_t global_sum; // Results from each process will be summed into this.
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
  mpf_t sum; // Local sum for each process
  mpf_init2(sum, d);

  mpf_t numerator; // Fractions numerator, always 1
  mpf_init2(numerator, d);
  mpf_set_d(numerator, 1.0);

  mpf_t denominator; // Fractions denominator, equals i
  mpf_init2(denominator, d);

  mpf_t frac;
  mpf_init2(frac, d);

  for(i = step[id]; i < step[id+1]; i++) {
    mpf_set_d(denominator, i);
    // gmp_printf("denom: %.Ff\n", denominator);
    mpf_div(frac, numerator, denominator); // Set frac = numerator/denominator
    // gmp_printf("frac: %.Ff\n", frac);
    mpf_add(sum, sum, frac);
    // gmp_printf("%.Ff\n", sum);
    // gmp_printf("sum: %.*Ff, numer: %.*Ff, denom: %.*Ff frac: %.*Ff\n", sum, numerator, denominator, frac);
  }
  if(id == 0) {
    gmp_printf("%.Ff\n", sum);
  }
  MPI_Finalize();
  return 0;
}
