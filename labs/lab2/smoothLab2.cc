#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <sys/time.h>
#include "mpi.h"
using namespace std;

void initialize(float* x, int n){
	int nbounds = n + 2;
	srand(1);
	for (int i = 0; i < nbounds; ++i){
		for (int j = 0; j < nbounds; ++j){
			x[i*nbounds + j] = random() / (float) RAND_MAX;
		}
	}
}

void smooth(float* x, float* y, int n, float a, float b, float c, int rank, int nprocs, int nbounds){
	int count, size, k, i, j, ierr;
	float *localData, *globalData;
	
	size = (n*n)/nprocs;
	
	if (rank == 0){	
		globalData = (float*)malloc(n*n*sizeof(float));
		for(i=0; i<(n*n); i++){
		   	globalData[i] = y[i];
		}
	}		
	localData = (float*)malloc(size*sizeof(float));

	ierr = MPI_Bcast(x, nbounds*nbounds, MPI_FLOAT, 0, MPI_COMM_WORLD);
	ierr = MPI_Scatter(y, size, MPI_FLOAT, localData, size, MPI_FLOAT, 0, MPI_COMM_WORLD);
	for (j = 0; j < size ;j++){
		i = localData[j];
		localData[j] = a * (x[(i-nbounds) - 1] +
							x[(i-nbounds) + 1] +
							x[(i+nbounds) - 1] +
							x[(i+nbounds) + 1]) +
					   b * (x[i - 1] +
							x[i + 1] +
							x[(i-nbounds)] +
							x[(i+nbounds)]) +
					   c * (x[i]);
	}
	ierr = MPI_Gather(localData, size, MPI_FLOAT, y, size, MPI_FLOAT, 0, MPI_COMM_WORLD);
	
	/*if (rank == 0){
		cout << endl;
		for (i=0; i<n; i++){
			for (j=0; j<n; j++){
				cout << y[i*n + j] << " ";
			}
		cout << endl;
		}
	cout << endl << endl;
	}*/
}

void count(float* x, int n, float t, int &nb, int rank, int nprocs, int xory)
{
	float* test = new float[n*n];
	int mb = 0, j, i;
	nb = 0;
	test = new float[n*n];	
	if (rank == 0){
		if (xory == 1){
			for (i=0; i<n; i++){
				for (j=0; j<n; j++){
					test[i*n + j] = x[(i+1)*(n+2) + (j+1)];
				}
			}
		} else{
			test = x;
		}
		for (i = 0; i <= n; i++){
			for (j = 0; j <= n; j++){
				if (test[i*n+j] < t){
					nb++;
				}
			}
		}
	}
}

void firstParallelRegion(int rank){
	cout << "Hello! I'm node " << rank << ", nice to meet you!" << endl;	
	MPI_Barrier(MPI_COMM_WORLD);
}

int main(int argc, char* argv[]){
	double time0, time1, time2, time3, time4, time5, time6, time7;
	int nprocs, rank, ierr;
	float *x, *y, a = .05, b = .1, c = .4, t = .1;
	int n = 32768/2, nbounds = n+2, nbx = 0, nby = 0, i, j;
	ierr = MPI_Init(&argc, &argv);
	ierr = MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
	ierr = MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	
	
	if (rank == 0) time0 = MPI_Wtime();
		x = new float[nbounds*nbounds];
	if (rank == 0){
		time1 = MPI_Wtime();
		y = new float[n*n];
		for (i=0; i<n; i++){
			for (j=0; j<n; j++){
				y[i*n+j] = (i+1)*(n+2) + (j+1);
			}
		}
		time2 = MPI_Wtime();
		initialize(x,n);
	}
	if (rank == 0) time3 = MPI_Wtime();
		firstParallelRegion(rank);
	if (rank == 0) time4 = MPI_Wtime();
		smooth(x, y, n, a, b, c, rank, nprocs, nbounds);
	if (rank == 0){
	   	time5 = MPI_Wtime();
		count(x, n, t, nbx, rank, nprocs, 1);
		time6 = MPI_Wtime();
		count(y, n, t, nby, rank, nprocs, 2);
		time7 = MPI_Wtime();
	}

	
	ierr = MPI_Finalize();
	if (rank == 0){
		printf("\n");
		printf("Summary\n");
		printf("-------\n");
		printf("Number of task                           :: %d\n", nprocs);
		printf("Number of elements in a row/column       :: %d\n", n+2);
		printf("Number of inner elements in a row/column :: %d\n", n);
		printf("Total number of elements                 :: %d\n", (n+2)*(n+2));
		printf("Total number of inner elements           :: %d\n", n*n);
		printf("Memory (GB) used per array               :: %g\n", (n+2)*(n+2)*sizeof(float) / (float)(1024*1024*1024));
		printf("Threshold                                :: %g\n", t);
		printf("Smoothing constants (a, b, c)            :: %g %g %g\n", a,b,c);
		printf("Number of elements below threshold (X)   :: %d\n", nbx);
		printf("Fraction of elements below threshold     :: %g\n", nbx / (float)(n*n));
		printf("Number of elements below threshold (Y)   :: %d\n", nby);
		printf("Fraction of elements below threshold     :: %g\n", nby / (float)(n*n)); 
		
		printf("CPU: Alloc-X :: %1.3f\n", time1 - time0);
		printf("CPU: Alloc-Y :: %1.3f\n", time2 - time1);
		printf("CPU: Init-X :: %1.3f\n", time3 - time2);
		printf("CPU: Smooth :: %1.3f\n", time5 - time4);
		printf("CPU: Count-X :: %1.3f\n", time6 - time5);
		printf("CPU: Count-Y :: %1.3f\n", time7 - time6);
	}
}
