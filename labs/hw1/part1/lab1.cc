#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <string>
#include <sys/time.h>
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

void smooth(float* x, float* y, int n, float a, float b, float c){
	int nbounds = n + 2;
	#pragma omp parallel for schedule(dynamic,100)
	for (int i = 1; i <= n; ++i){
		#pragma omp parallel for
		for (int j = 1; j <= n; ++j){
			y[i*nbounds + j] = a * (x[(i-1)*nbounds + (j-1)] +
									x[(i-1)*nbounds + (j+1)] +
									x[(i+1)*nbounds + (j-1)] +
									x[(i+1)*nbounds + (j+1)]) +
							   b * (x[i*nbounds + (j-1)] +
									x[i*nbounds + (j+1)] +
									x[(i-1)*nbounds + j] +
									x[(i+1)*nbounds + j]) +
							   c * (x[i*nbounds + j]);
		}
	}
}

void count(float* x, int n, float t, int &nb)
{
	int mb = 0;
	nb = 0;
	
	int nbounds = n+2;
	#pragma omp parallel for reduction(+:nb) private(mb)
	for (int i=1; i <= n; ++i){
		mb = 0;
		#pragma omp parallel for reduction(+:mb) 
		for (int j=1; j <= n; ++j){
			if (x[i*nbounds + j] < t){
				mb = mb + 1;
			}
		}
		nb = nb + mb; 
	}
}

void firstParallelRegion(){
	#pragma omp parallel
	{
		printf("This is thread %d\n", omp_get_thread_num());
	}
}

int main(){
	int mts = 8; omp_set_num_threads(mts);
	float *x, *y, a = .05, b = .1, c = .4, t = .1;
	int n = 32768;
    int nbx, nby;


	double time0 = omp_get_wtime();
		x = new float[(n+2)*(n+2)];
	double time1 = omp_get_wtime();
		y = new float[(n+2)*(n+2)];
	double time2 = omp_get_wtime();	
		initialize(x,n);
	double time3 = omp_get_wtime();
		firstParallelRegion();
	double time4 = omp_get_wtime();		
		smooth(x,y,n,a,b,c);
	double time5 = omp_get_wtime();	
		count(x, n, t, nbx);
	double time6 = omp_get_wtime();	
		count(y, n, t, nby);
	double time7 = omp_get_wtime();

	printf("\n");
	printf("Summary\n");
	printf("-------\n");
	printf("Number of threads used in parallel       :: %d\n", mts);
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
