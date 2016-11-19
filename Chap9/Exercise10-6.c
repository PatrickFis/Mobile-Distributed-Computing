/*
 * Write a program to evaluate the definite integral
 * 4x^3 + xy^2 + 5y + yz + 6z dz dy dx from x = 0 to 4, y = 0 to 3, z = 0 to x+y.
 * Note: This integral evaluates to 5000.1.
 * The mean value theorem for triple integrals states that an integral is
 * approximately equal to volume * average of integral. This is implemented
 * by setting volume equal to (4.0 - 0.0) * (3.0 - 0.0) * (2.0 - 0.0) and
 * calculating the value of the integrand at n random points.
 *
 * The book does NOT specify that this should be implemented in a parallel
 * manner, as it does with every other problem that needs a parallel solution.
 * I implemented a parallel version, and did not include a regular version as
 * this program is based on Exercise 10-5. The volume of the integral was left
 * alone, and I evaluated the integral with respect to z in the f function to
 * because the upper limit of integration was x+y.
 *
 * Compile using mpicc -o Exercise10-6 Exercise10-6.c -lm
 * Run using mpirun -np p ./Exercise10-6 n
 */

#include <mpi.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "MyMPI.h"

double f(double x, double y, double z) { // Return the value of the integrand
  // return (4.0*x*x*x) + (x*y*y) + (5.0*y) + (y*z) + (6.0*z);
  return (4.0*x*x*x)*(z) + (x*y*y)*(z) + (5.0*y)*(z) + (y*z)*((x+y)/2.0) + (6.0*z)*((x+y)/2.0);
}

double randDouble() { // Return a random double between 0.0 and 1.0.
  return (double)rand() / (double)RAND_MAX;
}

int main(int argc, char *argv[]) {

  int id;
  int p;
  MPI_Comm worker_comm;

  void manager(int, int);
  void worker(int, int);

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &id);
  MPI_Comm_size(MPI_COMM_WORLD, &p);

  if(p < 2) {
    printf("Program needs at least two processes\n");
  }
  else {
    if(!id) /* Manager */ {
      MPI_Comm_split(MPI_COMM_WORLD, MPI_UNDEFINED, id, &worker_comm);
    }
  }

  // Number of samples
  int n = atoi(argv[1]);
  size = BLOCK_SIZE(id, p, n);
  // printf("size = %d, id = %d\n", size,id);
  int i;
  for(i = 0; i < size; i++) { // Calculate values from integral
    x = randDouble() * 4;
    y = randDouble() * 3;
    // z = randDouble() * 2;
    z = randDouble() * (x + y);
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
