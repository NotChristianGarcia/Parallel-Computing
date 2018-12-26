#include <iostream>
#include <omp.h>
#include <stdio.h>

int main(){
	printf("Hello, from Master\n");

	#pragma omp parallel
		{
			printf("id%d\n",omp_get_thread_num());
		}
}
