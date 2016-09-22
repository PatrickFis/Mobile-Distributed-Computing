/*
* Write a parallel program that prints a hello, world message from each process,
* along with the processes id.
*/

/*
* Patrick Fischer
* 8/29/2016
* CS 3333, Exercise 4-6
*/
#include <mpi.h>
#include <stdio.h>

void HelloWorld(int id) {
  printf("hello, world, from process %d\n", id);
}
int main(int argc, char *argv[]) {
  int i;
  int id; // Processor rank
  int p; // Number of processes
  void HelloWorld(int id);
  MPI_Init (&argc, &argv); // Sets up system to use MPI
  MPI_Comm_rank(MPI_COMM_WORLD, &id);
  MPI_Comm_size(MPI_COMM_WORLD, &p);
  HelloWorld(id);
  fflush(stdout);
  MPI_Finalize();
  return 0;
}
