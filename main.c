/* Authors - Dan Ram and Ori Asulin  I.Ds 305015521, 308459304*/
/* Project - Calculating primitive Pythagorean triples */
/* Description - This is the main file which calls every other function which this project uses */




#include "threads.h"
#include "GeneralFunctions.h"


int main(int argc, char *argv[])
{
	extern int **SORTED_ARRAY;
	extern int real_buff;
	int max_number = atoi(argv[1]); // max_number should be initialize to the value of the corresponding argument of the main
	int num_of_threads = atoi(argv[2]); // num_of_threads should be initialized to the value of the corresponding argument of the main
	int *buffer_size = NULL;
	buffer_size = (int*)calloc(1, sizeof(int));

	extern int **OUTPUT_BUFFER;
	extern char *outfile_path;
	int index = 0;
	*buffer_size = atoi(argv[3]);
	real_buff = *buffer_size;
	Create_Read_and_Write_Semaphore(atoi(argv[3]));
		//INITALIZE BUFFER AND SORTED ARRAY
	InitMemory(buffer_size,argv[4]);
	
	create_threads_mutex_and_semaphores(max_number, num_of_threads, buffer_size);
	//PrintArr(&OUTPUT_BUFFER, real_buff);
	/* Print the primitive triples to the defined file */
	
	print_to_file(argv[4]);
	FinalRelease(SORTED_ARRAY, OUTPUT_BUFFER,buffer_size);
	
	return (0);
}




