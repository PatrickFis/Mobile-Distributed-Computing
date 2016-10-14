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
  double global_sum; // Results from each process will be summed into this.
  MPI_Init(&argc, &argv);

  /* Start the timer */
  MPI_Barrier(MPI_COMM_WORLD);
  elapsed_time = -MPI_Wtime();
  MPI_Comm_rank(MPI_COMM_WORLD, &id);
  MPI_Comm_size(MPI_COMM_WORLD, &p);

  if(argc != 3) {
    if(!id) printf("Command line: %s <n> <d>\n", argv[0]);
    MPI_Finalize();
    exit(1);
  }
  n = atoi(argv[1]);
  d = atoi(argv[2]);

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
  double sum = 0.0; // Local sum for each process

  for(i = step[id]; i < step[id+1]; i++) {
    double frac = 1/i;
    sum += frac;
  }
  if(id == 0) {
    printf("sum = %.100f\n", sum);
  }
  MPI_Finalize();
  return 0;
}
