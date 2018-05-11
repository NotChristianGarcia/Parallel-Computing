#include <mpi.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

int main(int argc, char ***argv){
	int rank, size;
	float randomfraction, maxRAND = 0, *randArray;

	MPI_Init(&argc, argv);

	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);

	// Initialize the random number generator
	randArray = (float *) malloc(size*sizeof(float));
	srand((int)(rank*(double)RAND_MAX/size));
	randomfraction = ((rand() / (double)RAND_MAX)); // compute a random number
	
	cout << "task: " << rank << " randomfraction: " << randomfraction << endl;

	MPI_Reduce(&randomfraction, &maxRAND, 1, MPI_FLOAT, MPI_MAX, 0, MPI_COMM_WORLD);

	if (rank == 0){
		cout << "The Largest Value is::" << maxRAND << endl;
	}

	MPI_Finalize();
}
