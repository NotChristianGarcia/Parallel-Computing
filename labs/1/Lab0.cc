#include <iostream>
#include <stdlib.h>
using namespace std;

void initialize(float* x, float n){
	int nbounds = n + 2;
	for (int i = 0; i < nbounds; i++){
		for (int j = 0; j < nbounds; j++){
			x[i*nbounds + j] = random() / (float) RAND_MAX;
		}
	}
}

void smooth(float* x, float* y, float n, float a, float b, float c){
	int nbounds = n + 2;
	for (int i = 1; i < n; i++){
		for (int j = 1; j < n; j++){
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

void count( float* x, int n, float t, int &nbx )
{
	 nbx = 0;
	  int n2 = n+2;
	   for (int i=1; i <= n; ++i)
		    for (int j=1; j <= n; ++j)
				 if (x[i*n2 + j] < t)
					  ++nbx;
} 

int main(){
	float *x, *y, a = .05, b = .1, c = .4, t = .1;
	int n = 1 << 14; //bit shift to the left once to get 2^14 = 16384 v. quickly
	x = new float[(n+2)*(n+2)];
	y = new float[(n+2)*(n+2)];

	initialize(x,n);
	smooth(x,y,n,a,b,c);
	count(x, n, t, nbx);	
	count(y, n, t, nby); 

	printf("\n");
	printf("Summary\n");
	printf("-------\n");
	printf("Number of elements in a row/column :: %d\n", n+2);
	printf("Number of inner elements in a row/column :: %d\n", n);
	printf("Total number of elements :: %d\n", (n+2)*(n+2));
	printf("Total number of inner elements :: %d\n", n*n);
	printf("Memory (GB) used per array :: %g\n", (n+2)*(n+2)*sizeof(float) / (float)(1024*1024*1024));
	printf("Threshold :: %g\n", t);
	printf("Smoothing constants (a, b, c) :: %g %g %g\n", a,b,c);
	printf("Number of elements below threshold (X) :: %d\n", nbx);
	printf("Fraction of elements below threshold :: %g\n", nbx / (float)(n*n));
	printf("Number of elements below threshold (Y) :: %d\n", nby);
	printf("Fraction of elements below threshold :: %g\n", nby / (float)(n*n)); 
}
