#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "mpi.h"

using namespace std;

void main(int argc, char* argv[]){
	double n = 10000000, x, y, z, pi;
	int i, count = 0;
	srand(time(NULL));

	for (i = 0; i<n; i++){
		x = (double)random()/RAND_MAX;
		y = (double)random()/RAND_MAX;
		z = sqrt((x*x)+(y*y));
		//check to see if point is in unit circle
		if (z <= 1){
			++count;
		}
	}
	pi = ((double)count/n)*4.0;
	printf("Pi: %f\n", pi);
}
