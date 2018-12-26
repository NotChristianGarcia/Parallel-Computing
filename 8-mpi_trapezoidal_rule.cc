// Solves trapezoid rule on multiple threads using Open MPI
#include <iostream>
#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

using namespace std;

void main(int argc, char* arv[]){
	int ierr, iam, np;
	MPI_COMM comm = MPI_COMM_WORLD;
	MPI_Status status;
	ierr = MPI_Init(&argc, &argv);
	ierr = MPI_Comm_size(comm, &nranks);
	ierr = MPI_Comm_rank(comm, &rank);

	for (i = rank; i < N; i+=size){
		x2 = d2 * i * i;
		result += sqrt(1-x2);
	}

	ierr = MPI_Reduce(&result, &sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
	ierr = MPI_Finalize();
}
