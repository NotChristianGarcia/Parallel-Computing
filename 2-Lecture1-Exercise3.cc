#include <iostream>
#include "mpi.h"
using namespace std;

int main(int argc, char* argv[]){
	printf("Hello World!\n");
	MPI_Init(&argc, &argv);
	char proc_name[MPI_MAX_PROCESSOR_NAME];
	int name_len;
	MPI_Get_processor_name(proc_name, &name_len);
	printf("Processor %s: Hello World!\n", proc_name);
	MPI_Finalize();
	printf("Hello World!\n");
}
