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

void my_bcast(void* data, int count, MPI_Datatype datatype, int root,
              MPI_Comm communicator) {
    int world_rank;
    MPI_Comm_rank(communicator, &world_rank);
    int world_size;
    MPI_Comm_size(communicator, &world_size);

    if (world_rank == root) {
        // If we are the root process, send our data to everyone
        int i;
        for (i = 0; i < world_size; i++) {
            if (i != world_rank) {
                MPI_Send(data, count, datatype, i, 0, communicator);
            }
        }
    } else {
        // If we are a receiver process, receive the data from the root
        MPI_Recv(data, count, datatype, root, 0, communicator,
                 MPI_STATUS_IGNORE);
    }
}


int main(int argc, char *argv[]) {
  int id;
  int p;
  double elapsed_time;
  double test_time; // Used to compare my_bcast to built in function
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
    my_bcast(&changeMe, 1, MPI_INT, 0, MPI_COMM_WORLD);
    my_bcast(&changeMe2, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    my_bcast(&changeMe3, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);
    my_bcast(&changeMe4, 1, MPI_LONG, 0, MPI_COMM_WORLD);
    my_bcast(&changeMe5, 1, MPI_CHAR, 0, MPI_COMM_WORLD);
    my_bcast(&changeMe6, 1, MPI_LONG_DOUBLE, 0, MPI_COMM_WORLD);
  }
  else {
    my_bcast(&changeMe, 1, MPI_INT, 0, MPI_COMM_WORLD);
    my_bcast(&changeMe2, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    my_bcast(&changeMe3, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);
    my_bcast(&changeMe4, 1, MPI_LONG, 0, MPI_COMM_WORLD);
    my_bcast(&changeMe5, 1, MPI_CHAR, 0, MPI_COMM_WORLD);
    my_bcast(&changeMe6, 1, MPI_LONG_DOUBLE, 0, MPI_COMM_WORLD);
  }
     elapsed_time += MPI_Wtime();
     test_time = MPI_Wtime();
  printf("changeMe = %d, changeMe2 = %f, changeMe3 = %f,changeMe4 = %ld, changeMe5 = %c, changeMe6 = %Lf,id = %d\n",
          changeMe,changeMe2,changeMe3,changeMe4,
          changeMe5,changeMe6,id);
  // printf("changeMe = %d\n", changeMe);
  if(id == 0) {
    changeMe = 2000;
    changeMe2 = 200.01;
    changeMe3 = 20.1010101;
    changeMe4 = 229301293;
    changeMe5 = 'M';
    changeMe6 = 29304329401324.18911322211;
    MPI_Bcast(&changeMe, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&changeMe2, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(&changeMe3, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&changeMe4, 1, MPI_LONG, 0, MPI_COMM_WORLD);
    MPI_Bcast(&changeMe5, 1, MPI_CHAR, 0, MPI_COMM_WORLD);
    MPI_Bcast(&changeMe6, 1, MPI_LONG_DOUBLE, 0, MPI_COMM_WORLD);
  }
  else {
      test_time += MPI_Wtime();
    printf("changeMe = %d, changeMe2 = %f, changeMe3 = %f,changeMe4 = %ld, changeMe5 = %c, changeMe6 = %Lf,id = %d\n",
            changeMe,changeMe2,changeMe3,changeMe4,
            changeMe5,changeMe6,id);
    printf("test_time = %10.6f\n", test_time);
  }
  MPI_Finalize();
  if(!id) printf("elapsed_time = %10.6f\n", elapsed_time);
  return 0;
}
