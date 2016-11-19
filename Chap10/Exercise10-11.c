/*
 * Write a program solve the room assignment problem from Section 10.5.4.
 * Assume n = 20 and T = 1. Use a random number generator to construct matrix
 * D. Each entry should be a uniform random variable between 0 and 10. Each
 * process should solve the problem for the same matrix D, but with different
 * seeds for the random number generator. Output will show the original array of
 * room assignments along with a rounded version of the array of roommate
 * incompatibilities. Following this will be the room assignments found by each
 * process. This was how I understood the problem, as each process is using a
 * different seed for the random number generator, so different processes
 * should calculate different results.
 *
 * Compile using mpicc -o Exercise10-11 Exercise10-11.c -lm
 * Run using mpirun -np p ./Exercise10-11
 */

#include <mpi.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "MyMPI.h"


double randDouble() { // Return a random double between 0.0 and 1.0.
  return (double)rand() / (double)RAND_MAX;
}

void shuffle(int *array, size_t n)
{ // This is an algorithm I found to shuffle arrays... using it to randomize the a array.
    if (n > 1)
    {
        size_t i;
        for (i = 0; i < n - 1; i++)
        {
          size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
          int t = array[j];
          array[j] = array[i];
          array[i] = t;
        }
    }
}
void swap(int *array, int i, int j) {
  int tmp = array[i];
  array[i] = array[j];
  array[j] = tmp;
}
int main(int argc, char *argv[]) {
  srand((unsigned)time(NULL)); // Set the random number seed
  int id;
  int p;
  int c1, c2; // Two people involved in a room swap
  double d[20][20]; // n x n matrix containing roommate incompatibilities
  double sum = 0.0; // Sum of dislikes of best solution found so far
  double new__sum = 0.0; // Sum of dislikes of newly generated solution
  double t = 1.0; // Temperature
  double elapsed_time;
  int a[20]; // n element array containing room assignments
  int i, j;
  MPI_Status status;

  MPI_Init(&argc, &argv);

  /* Start the timer */
  MPI_Barrier(MPI_COMM_WORLD);
  elapsed_time = -MPI_Wtime();
  MPI_Comm_rank(MPI_COMM_WORLD, &id);
  MPI_Comm_size(MPI_COMM_WORLD, &p);

  // Assign roommates randomly
  for(i = 0; i < 20; i++) {
    a[i] = i/2;
  }
  shuffle(a, 20);
  // Using a random number generator to populate D, which will be filled with
  // a uniform random variable between 0 and 10. This array will be the same
  // for every process.
  for(i = 0; i < 20; i++) {
    for(j = 0; j < 20; j++) {
      d[i][j] = randDouble() * 10;
    }
  }

  for(i = 0; i < 20; i++) { // Calculate starting sum.
    for(j = 0; j < 20; j++) {
      if(a[i] == a[j]) {
        sum = sum + d[i][j];
      }
    }
  }
  // printf("sum = %f, id = %d\n", sum, id);
  if(!id) {
    // Print the original room assignments and the rounded table of roommate incompatibilities
    printf("Original room assignments:\n");
    for(i = 0; i < 20; i++) {
      printf("%d ", a[i]);
    }
    printf("\n");
    printf("Roommate incompatibilities(rounded to two decimals):\n");
    for(i = 0; i < 20; i++) {
      for(j = 0; j < 20; j++) {
        printf("%.2f ", d[i][j]);
      }
      printf("\n");
    }
    printf("\n");
  }

  if(id != 0) srand((unsigned)time(NULL)); // Reset the random number seed to something new for each process

  i = 0;
  while(i < 1000) {
    // Stop if no changes for 1000 iterations
    double u = 0.0; // Holds the uniform random variable for c1 and c2.
    while(1) { // Pick two people to swap rooms
      u = randDouble()*20;
      c1 = (int)floor(u);
      u = randDouble()*20;
      c2 = (int)floor(u);
      if(a[c1] != a[c2]) {
        break;
      }
    }
    // Calculate new__sum assuming c1 and c2 swap rooms
    int tmpSwp[20];
    int k;
    for(k = 0; k < 20; k++) {
      tmpSwp[k] = a[k];
    }
    swap(tmpSwp, c1, c2);
    new__sum = 0.0;
    for(j = 0; j < 20; j++) { // Calculate new__sum.
      for(k = 0; k < 20; k++) {
        if(tmpSwp[j] == tmpSwp[k]) {
          new__sum = new__sum + d[j][k];
        }
      }
    }
    if(new__sum < sum || u <= exp((sum - new__sum)/t)) {
      // exp = e ^ expression
      // Swap the two roommates
      swap(a, c1, c2);
      sum = new__sum;
      i = 0;
    }
    else {
      i++;
    }
    t = 0.999 * t;
  }
  // Print a and sum from each process.
  printf("Process ID: %d, room assignments:\n", id);
  for(i = 0; i < 20; i++) {
    printf("%d ", a[i]);
  }
  printf("\n");

  MPI_Finalize();
  return 0;
}
