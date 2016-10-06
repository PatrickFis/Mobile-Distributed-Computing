/*
 * Write a parallel Sieve of Eratosthenes program based upon a functional decomposition of the algorithm.
 * Suppose there are p processes finding primes up to n. (The program gets these parameters from the command line.)
 * In the first step, each process independently identifies primes up to sqrt(n).
 * In step two each process sieves the list of integers with 1/pth of the primes between 2 and sqrt(n).
 * During the third step the processes OR-reduce their arrays into a single array held by process 0.
 * In the last step process 0 counts the unmarked elements of the array and prints the prime number count.
 *
 */

 #include <mpi.h>
 #include <math.h>
 #include <stdio.h>
 #include "MyMPI.h"
 #define min(a,b) ((a<b)?(a):(b))

int main(int argc, char *argv[]) {

	double elapsed_time; // Parallel execution time
	int global_count; // Global prime coun
	int i;
	int id; // Process ID number
	char *marked; // Portion of 2, ... , 'n'
	char *low_primes; // Seperate array for each process that will contain 3, ... , sqrt(n)
  char *final_primes; // Array used by process 0. Will be OR reduced into.
	int n; // Sieving fron 2, ... , 'n'
	int p; // Number of processes
	int proc0_size; // Size of proc 0's subarray
	int size; // Elements in 'marked'

	MPI_Init(&argc, &argv);

	/* Start the timer */
	MPI_Barrier(MPI_COMM_WORLD);
	elapsed_time = -MPI_Wtime();
	MPI_Comm_rank(MPI_COMM_WORLD, &id);
	MPI_Comm_size(MPI_COMM_WORLD, &p);

	if(argc != 2) {
		if(!id) printf("Command line: %s <m>\n", argv[0]);
		MPI_Finalize();
		exit(1);
	}

	n = atoi(argv[1]);

  int sqrtN = (int)sqrt((double)n);
  low_primes = (char *) malloc(sqrtN); // This is the 3 ... sqrt(n) array.
  // This code gives each process a copy of primes below sqrt(n).
  for(i = 0; i <= sqrtN; i++) low_primes[i] = 0;
	int tmp_prime_index;
	for(i = 2; i <= sqrtN; i++) {
		tmp_prime_index = i;
		for(int j = tmp_prime_index*2; j <= sqrtN; j+=tmp_prime_index) {
			low_primes[j] = 1;
		}
	}
	// This would be better with a dynamic array
	int low_prime_count = 0;
	for(i = 2; i <= sqrtN; i++) {
		if(!low_primes[i]) {
			low_prime_count++;
//			printf("low_primes[i]: %d, i: %d\n",low_primes[i],i);
		}
	}
	int local_primes[low_prime_count];
	int c = 0;
	for(i = 2; i <= sqrtN; i++) {
		if(!low_primes[i]) {
			local_primes[c++] = i;
      // printf("prime: %d\n", i);
		}
	}
  // for(int i = 0; i < low_prime_count; i++) { // Debug
  //   printf("%d\n", local_primes[i]);
  // }
  int *primes_per_process; // This will hold the prime numbers that each process will sieve with.
  int local_count = 0;
  for(i = id; i < low_prime_count; i+=p) {
    printf("prime: %d, id: %d\n", local_primes[i], id); // Debug
    local_count++;
  }
  primes_per_process = (int*) malloc(sizeof(int)*local_count);
  c = 0;
  for(i = id; i < low_prime_count; i+=p) {
    primes_per_process[c++] = local_primes[i];
  }
  // for(i = 0; i < c; i++) { // Debug
  //   printf("prime: %d, id: %d\n", primes_per_process[i], id);
  // }

  marked = (char *) malloc(n); // This is where the actual sieving will take place
  for(i = 0; i < n; i++) marked[i] = 0;

  int prime;
  for(i = 0; i < c; i++) { // Will go through list of primes per process
    prime = primes_per_process[i];
    for(int j = prime*2; j < n; j+=prime) {
      marked[j] = 1;
    }
  }
  // for(i = 0; i < n; i++) { // Debug
  //   if(!marked[i]) printf("marked prime: %d, id: %d\n", i, id);
  // }
  final_primes = (char *) malloc(n);
  for(i = 0; i < n; i++) final_primes[i] = 0;
  if(id != 0)
  MPI_Reduce(&marked, &final_primes, 1, MPI_CHAR, MPI_LOR, 0, MPI_COMM_WORLD);
  // for(i = 0; i < n; i++) {
  //   printf("final_primes[i]: %d, i: %d, id: %d\n", final_primes[i], i, id);
  // }
  elapsed_time += MPI_Wtime();

  int count = 0;
  if(id==0) {
    for(i = 0; i < n; i++) {
      if(!final_primes[i]) count++;
      // printf("%d, ", i);
    }
    printf("\n%d primes are less than or equal to %d\n", count,n);
		printf("Total elapsed time: %10.6f\n", elapsed_time);
  }
  MPI_Finalize();
  return 0;
}
