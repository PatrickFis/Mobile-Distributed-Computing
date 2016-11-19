/*
 * Implement another version of function read_col_striped_matrix that requires
 * no calls to MPI functions. Each process opens the data file and reads its
 * portion of the matrix.
 */

#include <mpi.h>
#include <math.h>
#include <stdio.h>
#include "MyMPI.h"
#include "MyMPI.c"

typedef double dtype;
#define mpitype MPI_DOUBLE

int main(int argc, char *argv[]) {
  dtype **a;
  dtype *b;
  dtype *c_block;
  dtype *c;
  dtype *storage;
  int i, j;
  int id;
  int m;
  int n;
  int nprime;
  int p;
  int rows;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &id);
  MPI_Comm_size(MPI_COMM_WORLD, &p);

  read_row_striped_matrix (argv[1], (void *) &a, (void *) &storage, mpitype, &m, &n, MPI_COMM_WORLD);
  rows = BLOCK_SIZE(id, p, m);
  print_row_striped_matrix ((void **) a, mpitype, m, n, MPI_COMM_WORLD);
  // int sum = 0; // Local sum
  // int global_sum = 0; // Global sum
  // double elapsed_time;
  MPI_Finalize();
  return 0;
}
