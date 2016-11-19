/*
 * Write a program to evaluate the definite integral
 * 4x^3 + xy^2 + 5y + yz + 6z dz dy dx from x = 0 to 4, y = 0 to 3, z = 0 to 2.
 * Note: This integral evaluates to 2040.
 * The mean value theorem for triple integrals states that an integral is
 * approximately equal to volume * average of integral. This is implemented
 * by setting volume equal to (4.0 - 0.0) * (3.0 - 0.0) * (2.0 - 0.0) and
 * calculating the value of the integrand at n random points.
 *
 * The book does NOT specify that this should be implemented in a parallel
 * manner, as it does with every other problem that needs a parallel solution.
 * I decided to implement a regular version, the commented out main function,
 * and a parallel version. The parallel version is very simple, each process
 * is given an amount of samples to work with, and then all the samples are
 * reduced to the root process where some arithmetic is done.
 *
 * Compile using mpicc -o Exercise10-5 Exercise10-5.c -lm
 * Run using mpirun -np p ./Exercise10-5 n
 */

#include <mpi.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "MyMPI.h"

double f(double x, double y, double z) { // Return the value of the integrand
  return (4.0*x*x*x) + (x*y*y) + (5.0*y) + (y*z) + (6.0*z);
}

double randDouble() { // Return a random double between 0.0 and 1.0.
  return (double)rand() / (double)RAND_MAX;
}

// int main(int argc, char *argv[]) {
//   srand((unsigned)time(NULL));
//   double sum = 0.0;
//   double answer = 0.0;
//   double x, y, z;
//   int i;
//   int n; // Number of samples
//   double est; // Estimate of integral
//   if(argc != 2) {
//     printf("Correct command: %s <# samples>\n", argv[0]);
//     return -1;
//   }
//   n = atoi(argv[1]);
//   for(i = 0; i < n; i++) { // Set x, y, z to values appropriate for their
//     // respective ranges
//     double x = randDouble()*4;
//     double y = randDouble()*3;
//     double z = randDouble()*2;
//     // printf("x = %f y = %f z = %f\n", x,y,z);
//     sum += f(x, y, z);
//   }
//   sum /= n;
//   double volume = (4.0 * 3.0 * 2.0);
//   est = volume * sum;
//   printf("Estimate = %.5f\n", est);
//   return 0l
// }

int main(int argc, char *argv[]) {
  srand((unsigned)time(NULL)); // Set the random number seed
  int id;
  int p;
  double elapsed_time;
  double x, y, z; // Variables used for integrand
  double sum = 0.0; // Local sum
  double global_sum = 0.0; // Global sum
  double volume = (4.0 - 0.0) * (3.0 - 0.0) * (2.0 - 0.0);
  int size; // Number of samples for each process to use
  MPI_Status status;

  MPI_Init(&argc, &argv);

  /* Start the timer */
  MPI_Barrier(MPI_COMM_WORLD);
  elapsed_time = -MPI_Wtime();
  MPI_Comm_rank(MPI_COMM_WORLD, &id);
  MPI_Comm_size(MPI_COMM_WORLD, &p);

  if(argc != 2) {
    printf("Correct command: %s <# samples>\n", argv[0]);
    MPI_Finalize();
    exit(1);
  }

  // Number of samples
  int n = atoi(argv[1]);
  size = BLOCK_SIZE(id, p, n);
  // printf("size = %d, id = %d\n", size,id);
  int i;
  for(i = 0; i < size; i++) { // Calculate values from integral
    x = randDouble() * 4;
    y = randDouble() * 3;
    z = randDouble() * 2;
    sum += f(x, y, z);
  }
  MPI_Reduce(&sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

  /* Stop the timer */
  elapsed_time += MPI_Wtime();

  /* Print the results */
  
  if(!id) {
    global_sum /= n;
    global_sum *= volume;
    printf("Estimated value for integral using %d samples is %.5f\n", n,global_sum);
    printf("Total elapsed time: %10.6f\n", elapsed_time);
  }
  MPI_Finalize();
  return 0;
}
