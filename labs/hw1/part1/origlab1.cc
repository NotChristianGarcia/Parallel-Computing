#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <string>
#include <sys/time.h>
using namespace std;

class Timer{
public:
	Timer(): n(0) { }
	void start(string label){
		if (n < 20){ 
			labels[n] = label; times[2*n] = clock();
		} else { 
			cerr << "No more timers, " << label
				<< " will not be timed." << endl; 
		}
	}
	void stop(){
		times[2*n+1] = clock();
		n++;
	}
	void reset(){
		n = 0;
	}
	void print(){
		cout << endl;
		cout << "Action :: time/s Time resolution = " << 1.f/(float)CLOCKS_PER_SEC << endl;
 		cout << "------" << endl;
		for (int i=0; i < n; ++i){
			cout << labels[i] << " :: " << (times[2*i+1] - times[2*i+0])/(float)CLOCKS_PER_SEC << endl;
		}  
	}
private:
	std::string labels[20];
	float times[40];
	int n;
};

void initialize(float* x, float n){
	int nbounds = n + 2;
	for (int i = 0; i < nbounds; i++){
		for (int j = 0; j < nbounds; j++){
			x[i*nbounds + j] = random() / (float) RAND_MAX;
		}
	}
}

void smooth(float* x, float* y, int n, float a, float b, float c){
	int nbounds = n + 2;
	for (int i = 1; i < n; ++i){
		for (int j = 1; j < n; ++j){
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

void count( float* x, int n, float t, int &nb )
{
	nb = 0;
	int n2 = n+2;	
	for (int i=1; i <= n; ++i){
		for (int j=1; j <= n; ++j){
			if (x[i*n2 + j] < t){
				++nb;
			}
		}
	}
} 

int main(){	
	printf("This is thread %d\n", omp_get_thread_num());

	float *x, *y, a = .05, b = .1, c = .4, t = .1;
	int n = 32768/*n = 1 << 14*/, nbx, nby; //bit shift to the left once to get 2^14 = 16384 v. quickly
	Timer timer;
	
	timer.start("CPU: Alloc-X");
		x = new float[(n+2)*(n+2)];
	timer.stop();
	timer.start("CPU: Alloc-Y");
		y = new float[(n+2)*(n+2)];
	timer.stop();
	timer.start("CPU: Init-X");
		initialize(x,n);
	timer.stop();
	timer.start("CPU: Smooth");
		smooth(x,y,n,a,b,c);
	timer.stop();
	timer.start("CPU: Count-X");
		count(x, n, t, nbx);
	timer.stop();
	timer.start("CPU: Count-Y");
		count(y, n, t, nby);
	timer.stop();

	printf("\n");
	printf("Summary\n");
	printf("-------\n");
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

	timer.print();
}
