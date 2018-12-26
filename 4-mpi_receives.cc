// Sending and receiving multiple packets to assigned procs using Open MPI and the Recv call
#include <iostream>
#include <mpi.h>
using namespace std;

int main(int argc, char* argv[]){
	MPI_Comm comm = MPI_COMM_WORLD;
	MPI_Status status;
	MPI_Request request;
	int nranks, rank =-1, recTemp = -1, ierr, rank0 = -1, rank1 = -1, rank2 = -1, 
		rank3 = -1, rank4 = -1, rank5 = -1, rank6 = -1, rank7 = -1;
	ierr = MPI_Init(&argc, &argv);
	ierr = MPI_Comm_size(comm, &nranks);
	ierr = MPI_Comm_rank(comm, &rank);
	
	if (rank != 0) ierr = MPI_Isend(&rank, 1, MPI_INT, 0, 9, comm, &request);
	if (rank != 1) ierr = MPI_Isend(&rank, 1, MPI_INT, 1, 9, comm, &request);
	if (rank != 2) ierr = MPI_Isend(&rank, 1, MPI_INT, 2, 9, comm, &request);
	if (rank != 3) ierr = MPI_Isend(&rank, 1, MPI_INT, 3, 9, comm, &request);
	if (rank != 4) ierr = MPI_Isend(&rank, 1, MPI_INT, 4, 9, comm, &request);
	if (rank != 5) ierr = MPI_Isend(&rank, 1, MPI_INT, 5, 9, comm, &request);
	if (rank != 6) ierr = MPI_Isend(&rank, 1, MPI_INT, 6, 9, comm, &request);
	if (rank != 7) ierr = MPI_Isend(&rank, 1, MPI_INT, 7, 9, comm, &request);
	
	ierr = MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, comm, &status);
	if (status.MPI_SOURCE == 0 && rank != 0) ierr = MPI_Recv(&rank0, 1, MPI_INT, MPI_ANY_SOURCE, 9, comm, &status);
	if (status.MPI_SOURCE == 1 && rank != 1) ierr = MPI_Recv(&rank1, 1, MPI_INT, MPI_ANY_SOURCE, 9, comm, &status);
	if (status.MPI_SOURCE == 2 && rank != 2) ierr = MPI_Recv(&rank2, 1, MPI_INT, MPI_ANY_SOURCE, 9, comm, &status);
	if (status.MPI_SOURCE == 3 && rank != 3) ierr = MPI_Recv(&rank3, 1, MPI_INT, MPI_ANY_SOURCE, 9, comm, &status);
	if (status.MPI_SOURCE == 4 && rank != 4) ierr = MPI_Recv(&rank4, 1, MPI_INT, MPI_ANY_SOURCE, 9, comm, &status);
	if (status.MPI_SOURCE == 5 && rank != 5) ierr = MPI_Recv(&rank5, 1, MPI_INT, MPI_ANY_SOURCE, 9, comm, &status);
	if (status.MPI_SOURCE == 6 && rank != 6) ierr = MPI_Recv(&rank6, 1, MPI_INT, MPI_ANY_SOURCE, 9, comm, &status);
	if (status.MPI_SOURCE == 7 && rank != 7) ierr = MPI_Recv(&rank7, 1, MPI_INT, MPI_ANY_SOURCE, 9, comm, &status);

	ierr = MPI_Finalize();
	if (rank == 0)
		printf(" %d\n %d\n %d\n %d\n %d\n %d\n %d\n", rank1, rank2, rank3, rank4, rank5, rank6, rank7);
}
