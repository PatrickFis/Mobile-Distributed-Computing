/*
 * Implement another version of function read_col_striped_matrix that requires
 * no calls to MPI functions. Each process opens the data file and reads its
 * portion of the matrix.
 */

 #include <mpi.h>
 #include <math.h>
 #include <stdio.h>
 #include "MyMPI.h"

int main(int argc, char *argv[]) {
  int id;
  int p;
  // int sum = 0; // Local sum
  // int global_sum = 0; // Global sum
  // double elapsed_time;
  MPI_Status status;

  MPI_Init(&argc, &argv);

  /* Start the timer */
  MPI_Barrier(MPI_COMM_WORLD);
  elapsed_time = -MPI_Wtime();
  MPI_Comm_rank(MPI_COMM_WORLD, &id);
  MPI_Comm_size(MPI_COMM_WORLD, &p);
}
