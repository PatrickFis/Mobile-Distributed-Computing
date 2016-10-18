/*
 * Exercise 6-9 - Write your own version of MPI_Reduce using functions
 * MPI_Send and MPI_Recv. You may assume datatype = MPI_INT, operator = MPI_SUM,
 * and comm = MPI_COMM_WORLD.
 *
 * Technically this works... just it isn't a function. This would be much easier
 * if C had pass by reference capabilities.
 */
#include <mpi.h>
#include <math.h>
#include <stdio.h>
#include "MyMPI.h"
// void myReduce(int *sendBuf, int *recvBuf, int id, MPI_Status status) {
//   if(id == 0) {
//     MPI_Recv(&sendBuf, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
//     int tempSum;
//     tempSum = *sendBuf + *recvBuf;
//   }
//   else {
//     MPI_Send(&sendBuf, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
//   }
// }
void myReduce(int sendBuf, int *recvBuf, int numProcesses, int id, MPI_Status status) {
  if(id == 0) {
    int i;
    for(i = 1; i < numProcesses; i++) {
      MPI_Recv(&sendBuf, 1, MPI_INT, i, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
      *recvBuf += sendBuf;
    }
  }
  else {
    MPI_Send(&sendBuf, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
  }
}

int main(int argc, char *argv[]) {
  int id;
  int p;
  int sum = 0; // Local sum
  int global_sum = 0; // Global sum
  MPI_Status status;

  MPI_Init(&argc, &argv);

  /* Start the timer */
  MPI_Barrier(MPI_COMM_WORLD);
  MPI_Comm_rank(MPI_COMM_WORLD, &id);
  MPI_Comm_size(MPI_COMM_WORLD, &p);
  // if(id != 0) {
  //   sum = 1337;
  //   MPI_Send(&sum, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
  // }
  // else {
  //   for(int i = 1; i < p; i++) {
  //     MPI_Recv(&sum, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
  //     global_sum += sum;
  //   }
  // }
  if(id != 0) {
    sum = 1337;
    myReduce(sum, &global_sum, p, id, status);
  }
  else {
    myReduce(sum, &global_sum, p, id, status);
  }
  // printf("sum = %d, id = %d\n", sum,id);
  // myReduce(&sum, &global_sum, id, status);
  if(id == 0)printf("global_sum = %d\n", global_sum);
  MPI_Finalize();
  return 0;
}
