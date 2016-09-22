/*
* Write a parallel program to approximate the value of the definite integral of
* 4/(1+x^2) dx on the bounds [0, 1] using the rectangle rule. The value should
* equal pi.
*/

/*
* Patrick Fischer
* 9/6/2016
* CS 3333, Exercise 4-11
*/

#include <mpi.h>
#include <stdio.h>

#define INTERVALS 1000000

int main(int argc, char *argv[]) {
  double area; // Area under curve
  double ysum; // Sum of rectangle heights
  double xi; // Midpoint of interval
  int i;
  int id; // Processor rank
  int p; // Number of processes
  double proc_area; // Area found by this process

  MPI_Init(&argc, &argv); // Sets up system to use MPI
  MPI_Comm_rank(MPI_COMM_WORLD, &id);
  MPI_Comm_size(MPI_COMM_WORLD, &p);
  // Code below determines how much work each process will do
  int perProcess = INTERVALS / p;
  int remainder = INTERVALS / p;
  int proc_intervals[p+1]; // Each process will be assigned a chunk
  // of the integral, and this array will have the bounds in it
  for(i = 0; i < p+1; i++) {
    if(i < p)
      proc_intervals[i] = i*perProcess;
    else
      proc_intervals[i] = INTERVALS;
  }
  // for(i = 0; i < p+1; i++) {
  //   printf("%d\n", proc_intervals[i]);
  // }
  ysum = 0.0;
  for(i = proc_intervals[id]; i < proc_intervals[id + 1]; i++) {
    xi = (1.0/INTERVALS) * (i+0.5);
    ysum += 4.0/(1.0+xi*xi);
  }
  proc_area = ysum * (1.0/INTERVALS);
  MPI_Reduce(&proc_area, &area, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
  MPI_Finalize();
  if(id == 0) {
    printf("Area is %13.11f\n", area);
    return 0;
  }
}
