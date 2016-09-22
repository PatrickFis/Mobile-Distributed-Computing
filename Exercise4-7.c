/*
* Write a parallel program that computes the sum 1+2+...+p in the following manner:
* Each process i assigns the value i + 1 to an integer, and then the processes perform
* a sum reduction of these values. Process 0 should print the result of the reduction.
* As a way of double-checking the result, process 0 should print the value p(p+1)/2.
*/

/*
* Patrick Fischer
* 8/27/2016
* CS 3333, Exercise 4-7
*/
#include <mpi.h>
#include <stdio.h>

int p_rank_plus1(int id, int n) {
  int i = n+1;
  return i;
}
int main(int argc, char *argv[]) {
  int global_solution=0; // Stores final result
  int id; // Processor rank
  int p; // Number of processes
  int solution = 0; // Local solution for each process
  int p_rank_plus1(int, int);
  MPI_Init (&argc, &argv); // Sets up system to use MPI
  MPI_Comm_rank(MPI_COMM_WORLD, &id);
  MPI_Comm_size(MPI_COMM_WORLD, &p);

  solution = p_rank_plus1(id, id); // Solution = process rank + 1

  MPI_Reduce(&solution, &global_solution, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
  // MPI_Reduce will basically take the value of solution from each process and add it
  // to global_solution.
  // printf("Solution so far: %d\n", global_solution);
  printf("Process %d is done\n", id);
  fflush(stdout);
  MPI_Finalize(); // Clear memory, etc.
  if(id == 0) { // Only print out solution in process 0.
    printf("Global solution after reduction: %d\n", global_solution);
    printf("Sum should equal: %d\n",p*(p+1)/2);
  }
  return 0;

}
