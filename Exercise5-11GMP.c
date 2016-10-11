/*
* Exercise 5-11 is to write a program to calculate the sum of a simple harmonic progression.
* Since this assignment requires the use of arbitrary precision, I decided to use the GMP MP Bignum
* library to save some time. The program can be compiled by typing
* mpicc -o Exercise5-11 Exercise5-11GMP.c -lgmp. The last flag is to link the GMP library.
*
* mpf_t is a floating point number
* mpf_init(var) = set to 0 with default precision
* mpf_init2(var, num) set to 0 with num bits of precision
* char * mpf_get_str (char *str, mp_exp_t *expptr, int base, size_t n_digits, const mpf_t op) converts floats
*/

#include <mpi.h>
#include <math.h>
#include <stdio.h>
#include <gmp.h>
#include "MyMPI.h"
// So I might just need to convert the local sum to a string and then use MPI_send to send it to process 0.
// It will need to be converted back to a floating point number and then added up.

// I might need to write a custom reduce with some sort of typedef struct made up of
// just one mpf_t.

// I might be able to do this with MPI_Gather instead. I can send an array containing the local sum as a string and the exponent.
void myProd(mpf_t *local_sum, mpf_t *global_sum, int *len, MPI_Datatype *dptr) {

}
int main(int argc, char *argv[]) {
  double elapsed_time; // Parallel execution time
  double i;
  int id; // Process ID number
  int p; // Number of processes
  int n; // Range of series
  int d; // Precision of series
  mpf_t global_sum; // Results from each process will be summed into this.
  mpf_init2(global_sum, 1000);
  mp_exp_t exponent; // Holds the exponent used for converting floats into strings.
  MPI_Status status;
  char *sump0, *ep0, *sump1, *ep1;
  MPI_Init(&argc, &argv);

  /* Start the timer */
  MPI_Barrier(MPI_COMM_WORLD);
  elapsed_time = -MPI_Wtime();
  MPI_Comm_rank(MPI_COMM_WORLD, &id);
  MPI_Comm_size(MPI_COMM_WORLD, &p);
  char *local_vals[p]; // Will be used to store the local sums as strings

  if(id == 0) { // Get n and d, broadcast to other processes.
    printf("Enter n: ");
    scanf("%d", &n);
    printf("Enter d: ");
    scanf("%d", &d);
  }
  MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&d, 1, MPI_INT, 0, MPI_COMM_WORLD);
  // printf("%d %d %d\n", n,d,id);

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
  mpf_t sum; // Local sum for each process
  mpf_init2(sum, 1000);

  mpf_t numerator; // Fractions numerator, always 1
  mpf_init2(numerator, 1000);
  mpf_set_d(numerator, 1.0);

  mpf_t denominator; // Fractions denominator, equals i
  mpf_init2(denominator, 1000);

  mpf_t frac;
  mpf_init2(frac, 1000);

  for(i = step[id]; i < step[id+1]; i++) {
    mpf_set_d(denominator, i);
    // gmp_printf("denom: %.Ff\n", denominator);
    mpf_div(frac, numerator, denominator); // Set frac = numerator/denominator
    // gmp_printf("frac: %.Ff\n", frac);
    mpf_add(sum, sum, frac);
    // gmp_printf("sum = %.Ff\n", sum);
    // gmp_printf("sum: %.*Ff, numer: %.*Ff, denom: %.*Ff frac: %.*Ff\n", sum, numerator, denominator, frac);
  }
  char *sumString;
  sumString = mpf_get_str(NULL, &exponent, 10, 0, sum);
  char *sumExponent[2];
  sumExponent[0] = sumString;
  int convert = exponent; // This is a horrible hack. Had to convert the exponent from a long int * to a character pointer.
  char buffer[32];
  int ret = snprintf(buffer, sizeof(buffer), "%d", convert);
  sumExponent[1] = buffer;
  // printf("sumString = %s, exponent = %s\n", sumExponent[0], sumExponent[1]);
  MPI_Gather(sumString, 1, MPI_CHAR, sendArray, 1, MPI_CHAR, 0, MPI_COMM_WORLD);
  if(id == 0) {
    for(int i = 0; i < p*2; i++) {
      char* temp = sendArray[i];
      printf("%s\n",sendArray[i]);
      // printf("%s\n",sendArray[i]);
    }
  }
  // printf("sumString = %s, exp = %d\n", sumString, exponent);
  // local_vals[id] = sumString; // Store sum as a string
  // MPI_Barrier(MPI_COMM_WORLD);
  // if(id != 0) {
  //   MPI_Send(&sumString, 1, MPI_CHAR, 0, 1, MPI_COMM_WORLD);
  //   MPI_Send(&exponent, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
  //   printf("sumString = %s, exp = %d, id=%d\n", sumString, exponent, id);
  // }
  // else {
  //   MPI_Recv(&sumString, 1, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
  //   MPI_Recv(&exponent, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
  //   printf("sumString = %s, exp = %d, id=%d\n", sumString, exponent, id);
  //   gmp_printf("sumString=%.Ff\n",sumString);
  // }
  if(id == 0) {
    gmp_printf("%.Ff\n", global_sum);
  }
  MPI_Finalize();
  return 0;
}
