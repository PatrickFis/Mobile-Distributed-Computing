/*
* Exercise 6-10 - Write your own version of MPI_Bcast using MPI_Send and
* MPI_Recv.
* Appears to be working when there are only two processes. Probably a pointer
* issue.
*/
#include <mpi.h>
#include <math.h>
#include <stdio.h>
#include "MyMPI.h"
int myBcast(int buffer,
            int count,
            MPI_Datatype datatype,
            int root,
            MPI_Comm comm,
            int numProcesses,
            MPI_Status status) {
  /*
  * buffer - Address of 1st broadcast element
  * count - Number of elements to broadcast
  * datatype - Type of elements to broadcast
  * root - ID of process doing the broadcast
  * comm - Communicator
  * numProcesses - Number of processes
  * status - Used for MPI_Recv.
  */
  if(root == 0) {
    for(int i = 1; i < numProcesses; i++) {
      MPI_Send(&buffer, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
      // printf("buffer: %d, i: %d\n", buffer,i);
    }
  }
  else {
    MPI_Recv(&buffer, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    return buffer;
  }
  // switch(root) {
  //   case 0:
  //   // If process 0 broadcast to other processes.
  //   for(int i = 1; i < numProcesses; i++) {
  //     // printf("i=%d\n",i);
  //     MPI_Send(&buffer, count, datatype, i, 0, comm);
  //   }
  //   default:
  //   // If any other process receive data from process 0.
  //   // printf("default");
  //   MPI_Recv(&buffer, count, datatype, 0, 0, comm,  &status);
  // }
}
int main(int argc, char *argv[]) {
  int id;
  int p;
  double elapsed_time;
  int changeMe;
  MPI_Status status;

  MPI_Init(&argc, &argv);

  /* Start the timer */
  MPI_Barrier(MPI_COMM_WORLD);
  elapsed_time = -MPI_Wtime();
  MPI_Comm_rank(MPI_COMM_WORLD, &id);
  MPI_Comm_size(MPI_COMM_WORLD, &p);
  // if(id == 1) {
  //   MPI_Recv(&changeMe, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
  //       myBcast(&changeMe, 1, MPI_INT, 1, MPI_COMM_WORLD, p, status);
  // }
  // if(id == 0) {
  //   changeMe = 1000;
  //   // for(int i = 1; i < p; i++) {
  //   //   MPI_Send(&changeMe, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
  //   // }
  //   // MPI_Send(&changeMe, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
  // }
  // else {
  //   MPI_Recv(&changeMe, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
  // }
    // myBcast(&changeMe, 1, MPI_INT, id, MPI_COMM_WORLD, p, status);
  // myBcast(&changeMe, 1, MPI_INT, id, MPI_COMM_WORLD, p, &status);

  // else {
  //     myBcast(&changeMe, 1, MPI_INT, id, MPI_COMM_WORLD, p, &status);
  // }
  if(id == 0) {
    changeMe = 1000;
    myBcast(changeMe, 1, MPI_INT, id, MPI_COMM_WORLD, p, status);
  }
  else {
    changeMe = myBcast(changeMe, 1, MPI_INT, id, MPI_COMM_WORLD, p, status);
  }

  printf("changeMe = %d, id = %d\n",changeMe,id);
  MPI_Finalize();
  return 0;
}
