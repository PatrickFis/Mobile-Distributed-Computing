/*
* Exercise 6-9 - Write your own version of MPI_Reduce using functions
* MPI_Send and MPI_Recv. You may assume datatype = MPI_INT, operator = MPI_SUM,
* and comm = MPI_COMM_WORLD.
*/
#include <mpi.h>
#include <math.h>
#include <stdio.h>
#include "MyMPI.h"

/*
 * If a process is the root process, then MPI_Recv will block until it recieves sendBuf from
 * process i. Since C is pass by value, recvBuf must be manipulated through
 * pointers.
 */
void myReduce(int sendBuf, int *recvBuf, int count, int numProcesses, int id, MPI_Status status) {
  if(id == 0) {
    int i;
    for(i = 1; i < numProcesses; i++) {
      MPI_Recv(&sendBuf, count, MPI_INT, i, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
      *recvBuf += sendBuf;
    }
  }
  else {
    MPI_Send(&sendBuf, count, MPI_INT, 0, 0, MPI_COMM_WORLD);
  }
}

int main(int argc, char *argv[]) {
  int id;
  int p;
  int sum = 0; // Local sum
  int global_sum = 0; // Global sum
  double elapsed_time;
  MPI_Status status;

  MPI_Init(&argc, &argv);

  /* Start the timer */
  MPI_Barrier(MPI_COMM_WORLD);
  elapsed_time = -MPI_Wtime();
  MPI_Comm_rank(MPI_COMM_WORLD, &id);
  MPI_Comm_size(MPI_COMM_WORLD, &p);
  if(id != 0) {
    int i;
    for(i = 1; i < 101; i++) {
      sum += i;
    }
  }
  myReduce(sum, &global_sum, 1, p, id, status);
  elapsed_time += MPI_Wtime();
  if(id == 0) {
    printf("global_sum = %d, elapsed_time = %f\n", global_sum,elapsed_time);
  }
  MPI_Finalize();
  return 0;
}
