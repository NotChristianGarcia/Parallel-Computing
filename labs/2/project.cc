#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <string>
#include <sys/time.h>
using namespace std;

/* Array:
0: Land/Surface
1: Food
2: Water

*/


class dataLayer{
	public:
		int n, nbounds, i, j, nb, mb;
		float t = .5, a, b, c;
		float *test, *notSmooth, *smooth, *blank;
		
		// For information layers
		dataLayer(int nput, float aput, float bput, float cput){
			n = nput;
			a = aput;
			b = bput;
			c = cput;
			nbounds = n + 2;
			infoDataInit();
			initialize();
			smoothing();
		}
		
		// Creating blank layers
		dataLayer(int nput){
			n = nput;
			nbounds = n + 2;
			blankDataInit();

		}

		void blankDataInit(){
			blank = new float[(nbounds)*(nbounds)];
			for (int i = 0; i < nbounds; i++){
				for (int j = 0; j < nbounds; ++j){
					notSmooth[i*nbounds + j] = 0;
				}
			}
		}

		void infoDataInit(){
			notSmooth = new float[(nbounds)*(nbounds)];
			smooth = new float[(nbounds)*(nbounds)];
		}
	
		void initialize(){
			srand(1);
			for (int i = 0; i < nbounds; i++){
				for (int j = 0; j < nbounds; ++j){
					notSmooth[i*nbounds + j] = random() / (float) RAND_MAX;
				}
			}
		}

		void smoothing(){
			#pragma omp parallel for schedule(dynamic,100)
			for (int i = 1; i <= n; ++i){
				#pragma omp parallel for
				for (int j = 1; j <= n; ++j){
					smooth[i*nbounds + j] =    a * (notSmooth[(i-1)*nbounds + (j-1)] +
													notSmooth[(i-1)*nbounds + (j+1)] +
													notSmooth[(i+1)*nbounds + (j-1)] +
													notSmooth[(i+1)*nbounds + (j+1)]) +
											   b * (notSmooth[i*nbounds + (j-1)] +
													notSmooth[i*nbounds + (j+1)] +
													notSmooth[(i-1)*nbounds + j] +
													notSmooth[(i+1)*nbounds + j]) +
											   c * (notSmooth[i*nbounds + j]);
				}
			}
		}
		// input 0 for notSmooth or 1 for Smooth
		int count(int smoothOrNah){
			nb = 0;
			if (smoothOrNah == 0){
				test = notSmooth;
			} else {
				test = smooth;
			}
			#pragma omp parallel for reduction(+:nb) private(mb)
			for (int i=1; i <= n; ++i){
				mb = 0;
				#pragma omp parallel for reduction(+:mb) 
				for (int j=1; j <= n; ++j){
					if (test[i*nbounds + j] < t){
						mb = mb + 1;
					}
				}
				nb = nb + mb; 
			}
			return nb;
		}
		
		void smoothCheck(){
			i = 600, j = 500;
			printf("%f %f %f\n%f %f %f\n%f %f %f\n",	smooth[(i-1)*nbounds + (j-1)], 	smooth[(i-1)*nbounds + j], 	smooth[(i-1)*nbounds + (j+1)],
														smooth[i*nbounds + (j-1)], 		smooth[i*nbounds + j], 		smooth[i*nbounds + (j+1)],
														smooth[(i+1)*nbounds + (j-1)], 	smooth[(i+1)*nbounds + j], 	smooth[(i+1)*nbounds + (j+1)]);
		}
		
		float* getInfoData(){
			return smooth;
		}
};

class creature{
	public:
		int x, y, n;
		float hunger;
		float *resourceData;
		bool dead, deadannounced;
		
		creature(int nput){
			n = nput;
			nbounds = n + 2;
			resources.getData();
			x = rand() % 1 + (n-1);
			y = rand() % 1 + (n-1);
			hunger = (rand() % 0 + 100)/100;
			dead = FALSE;
			deadannounced = FALSE;
		}

		void simulate(){
			if (deadannounced != FALSE && dead == TRUE){
				deadannounced == TRUE;
			}
		}

		void move(){
			
			
		}
		
		void death(){
			if (hunger <= .05){
				dead == TRUE;
			}
		}
};

void firstParallelRegion(){
	#pragma omp parallel
	{
		printf("This is thread %d\n", omp_get_thread_num());
	}
}

int main(){
	int mts = 8; omp_set_num_threads(mts);
	int n = 1000, nbx, nby;
	
	firstParallelRegion();
	dataLayer surface(n, .2, .4, .4);
	dataLayer resources(n, .2, .4, .4);
	nbx = resources.count(0);
	nby = resources.count(1);

	printf("\n");
	printf("Summary\n");
	printf("-------\n");
	printf("Number of threads used in parallel       :: %d\n", mts);
	printf("Number of inner elements in a row/column :: %d\n", n);
	printf("Number of elements below threshold (X)   :: %d\n", nbx);
	printf("Fraction of elements below threshold     :: %g\n", nbx / (float)(n*n));
	printf("Number of elements below threshold (Y)   :: %d\n", nby);
	printf("Fraction of elements below threshold     :: %g\n", nby / (float)(n*n)); 
	/*
	double time0 = omp_get_wtime();
	printf("CPU: Alloc-X :: %1.3f\n", time1 - time0);
	printf("CPU: Alloc-Y :: %1.3f\n", time2 - time1);
	printf("CPU: Init-X :: %1.3f\n", time3 - time2);
	printf("CPU: Smooth :: %1.3f\n", time5 - time4);
	printf("CPU: Count-X :: %1.3f\n", time6 - time5);
	printf("CPU: Count-Y :: %1.3f\n", time7 - time6);
	*/
}
