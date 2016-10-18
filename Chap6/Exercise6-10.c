/*
* Exercise 6-10 - Write your own version of MPI_Bcast using MPI_Send and
* MPI_Recv.
* I just wrote different functions for different data types. I had some trouble getting
* a generic version working.
*/
#include <mpi.h>
#include <math.h>
#include <stdio.h>
#include "MyMPI.h"
int myBcastInt(int buffer,
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
      MPI_Send(&buffer, count, datatype, i, 0, MPI_COMM_WORLD);
      // printf("buffer: %d, i: %d\n", buffer,i);
    }
  }
  else {
    MPI_Recv(&buffer, count, datatype, 0, 0, MPI_COMM_WORLD, &status);
    return buffer;
  }
}

long myBcastLong(long buffer,
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
      MPI_Send(&buffer, count, datatype, i, 0, MPI_COMM_WORLD);
      // printf("buffer: %d, i: %d\n", buffer,i);
    }
  }
  else {
    MPI_Recv(&buffer, count, datatype, 0, 0, MPI_COMM_WORLD, &status);
    return buffer;
  }
}

double myBcastDouble(double buffer,
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
      MPI_Send(&buffer, count, datatype, i, 0, MPI_COMM_WORLD);
      // printf("buffer: %d, i: %d\n", buffer,i);
    }
  }
  else {
    MPI_Recv(&buffer, count, datatype, 0, 0, MPI_COMM_WORLD, &status);
    return buffer;
  }
}

float myBcastFloat(float buffer,
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
      MPI_Send(&buffer, count, datatype, i, 0, MPI_COMM_WORLD);
      // printf("buffer: %d, i: %d\n", buffer,i);
    }
  }
  else {
    MPI_Recv(&buffer, count, datatype, 0, 0, MPI_COMM_WORLD, &status);
    return buffer;
  }
}

char myBcastChar(char buffer,
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
      MPI_Send(&buffer, count, datatype, i, 0, MPI_COMM_WORLD);
      // printf("buffer: %d, i: %d\n", buffer,i);
    }
  }
  else {
    MPI_Recv(&buffer, count, datatype, 0, 0, MPI_COMM_WORLD, &status);
    return buffer;
  }
}

long double myBcastLDouble(long double buffer,
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
      MPI_Send(&buffer, count, datatype, i, 0, MPI_COMM_WORLD);
      // printf("buffer: %d, i: %d\n", buffer,i);
    }
  }
  else {
    MPI_Recv(&buffer, count, datatype, 0, 0, MPI_COMM_WORLD, &status);
    return buffer;
  }
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
  double changeMe2;
  float changeMe3;
  long changeMe4;
  char changeMe5;
  long double changeMe6;
  if(id == 0) {
    changeMe = 1000;
    changeMe2 = 100.01;
    changeMe3 = 10.1010101;
    changeMe4 = 129301293;
    changeMe5 = 'C';
    changeMe6 = 19304329401324.18911322211;
    myBcastInt(changeMe, 1, MPI_INT, id, MPI_COMM_WORLD, p, status);
    myBcastDouble(changeMe2, 1, MPI_DOUBLE, id, MPI_COMM_WORLD, p, status);
    myBcastFloat(changeMe3, 1, MPI_FLOAT, id, MPI_COMM_WORLD, p, status);
    myBcastLong(changeMe4, 1, MPI_LONG, id, MPI_COMM_WORLD, p, status);
    myBcastChar(changeMe5, 1, MPI_CHAR, id, MPI_COMM_WORLD, p, status);
    myBcastLDouble(changeMe6, 1, MPI_LONG_DOUBLE, id, MPI_COMM_WORLD, p, status);
  }
  else {
     changeMe = myBcastInt(changeMe, 1, MPI_INT, id, MPI_COMM_WORLD, p, status);
     changeMe2 = myBcastDouble(changeMe2, 1, MPI_DOUBLE, id, MPI_COMM_WORLD, p, status);
     changeMe3 = myBcastFloat(changeMe3, 1, MPI_FLOAT, id, MPI_COMM_WORLD, p, status);
     changeMe4 = myBcastLong(changeMe4, 1, MPI_LONG, id, MPI_COMM_WORLD, p, status);
     changeMe5 = myBcastChar(changeMe5, 1, MPI_CHAR, id, MPI_COMM_WORLD, p, status);
     changeMe6 = myBcastLDouble(changeMe6, 1, MPI_LONG_DOUBLE, id, MPI_COMM_WORLD, p, status);
  }
     elapsed_time += MPI_Wtime();
  printf("changeMe = %d, changeMe2 = %f, changeMe3 = %f,changeMe4 = %ld, changeMe5 = %c, changeMe6 = %Lf,id = %d\n",
          changeMe,changeMe2,changeMe3,changeMe4,
          changeMe5,changeMe6,id);
  MPI_Finalize();
  if(!id) printf("elapsed_time = %10.6f\n", elapsed_time);
  return 0;
}
