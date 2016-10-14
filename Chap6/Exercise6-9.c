/*
 * Exercise 6-9 - Write your own version of MPI_Reduce using functions
 * MPI_Send and MPI_Recv. You may assume datatype = MPI_INT, operator = MPI_SUM,
 * and comm = MPI_COMM_WORLD.
 *
 */
#include <mpi.h>
#include <math.h>
#include <stdio.h>
#include "MyMPI.h"

int main(int argc, char *argv[]) {
  MPI_Init(&argc, &argv);

  /* Start the timer */
  MPI_Barrier(MPI_COMM_WORLD);
  elapsed_time = -MPI_Wtime();
  MPI_Comm_rank(MPI_COMM_WORLD, &id);
  MPI_Comm_size(MPI_COMM_WORLD, &p);

}
