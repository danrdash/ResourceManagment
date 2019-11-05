/* Description - This module contains the functions within the project */


#include "GeneralFunctions.h"
#include "threads.h"


//Variables

HANDLE *SortSemaphore=NULL;
HANDLE *FullSemaphore=NULL;
HANDLE *EmptySemaphore=NULL;


HANDLE *mutex_for_computations=NULL;
HANDLE *mutex_for_buffer_idx=NULL;


//OUTPUT_BUFFER declaration
int **OUTPUT_BUFFER=NULL;
int **SORTED_ARRAY=NULL;
//The set of numbers from which any anchor is drawn
int *nums_for_calculation=NULL;
int valid_bit = 0;
int thread_tag = 0;
int next_up = -1;
int real_buff;
char *outfile_path=NULL;




//Description - release buffer and sorted array
//Parameters - buffer and sorted array
//Return - returns 0 if succeeded
int FinalRelease(int ***SORTED_ARRAY1, int ***OUTPUT_BUFFER1,int *buffer_size)
{
	extern int **SORTED_ARRAY, **OUTPUT_BUFFER;
	int index = 0;//, **SORTED_ARRAY=*SORTED_ARRAY1, **OUTPUT_BUFFER=*OUTPUT_BUFFER1;
	for (int index = 0; index < *buffer_size; index++)
	{
		free(*(OUTPUT_BUFFER + index));

	}
	free(OUTPUT_BUFFER);
	for (index = 0; index < SORTED_INIT_SIZE; index++)
	{
		free(*(SORTED_ARRAY + index));

	}
	free(SORTED_ARRAY);
	return 0;
}
//Description - initalize buffer and sorted array and outpath_file
//Parameters - buffer and sorted array
//Return - returns 0 if succeeded
int InitMemory(int *buffer_size, char *output)
{
	extern int real_buff;
	extern char *outfile_path;
	extern int **OUTPUT_BUFFER, **SORTED_ARRAY;
	int index = 0;
	outfile_path = (char*)malloc(MAX_OUTPATH * sizeof(char));
	if (NULL == outfile_path) {
		ErrorPlan(outfile_path); //could not allocate memory
		exit(1);
	}
	strcpy(outfile_path, output);


	OUTPUT_BUFFER = (int **)malloc(real_buff * sizeof(int*));
	if (NULL == OUTPUT_BUFFER) {
		ErrorPlan(outfile_path); //could not allocate memory
		exit(1);
	}
	for (index = 0; index < real_buff; index++)
	{
		*(OUTPUT_BUFFER + index) = (int *)calloc(6, sizeof(int));
		*(*(OUTPUT_BUFFER + index) + 3) = 1;
		if (NULL == *(OUTPUT_BUFFER + index)) {
			ErrorPlan(outfile_path); //could not allocate memory
			exit(1);
		}
	}
	SORTED_ARRAY = (int **)malloc(SORTED_INIT_SIZE * sizeof(int*));
	if (NULL == SORTED_ARRAY) {
		ErrorPlan(outfile_path); //could not allocate memory
		exit(1);
	}
	for (index = 0; index < SORTED_INIT_SIZE; index++) {
		*(SORTED_ARRAY + index) = (int *)calloc(6, sizeof(int));
		if (NULL == (*(SORTED_ARRAY + index))) {
			ErrorPlan(outfile_path); //could not allocate memory
			exit(1);
		}
	}
	return 0;
}
//Description - write to output file and error message
//Parameters - outfile path
//Return - returns 0 if succeeded
int ErrorPlan(char *outpath)
{
	FILE *fp = (FILE*)malloc(sizeof(FILE));
	fopen_s(&fp, outpath, "w");
	fprintf(fp, "An error occured, program had to exit with exitCode 1. Thanks for running our code, Dan&Ori - xoxo <3");
	fclose(fp);
	free(fp);
	return 0;
}
//Description – This function prints all triples to the output file
//Parameters – A pointer to a string which states the path to the output file
//Returns – zero when successful

int print_to_file(char *path_to_file)
{
	extern int **SORTED_ARRAY;
	int idx = 0;
	FILE *fp_write = NULL;
	fopen_s(&fp_write, path_to_file, "w");
	if (fp_write == NULL) return FAILED;
	for (idx = 0; idx < SORTED_INIT_SIZE; idx++)
	{
		if (0 != SORTED_ARRAY[idx][0])
			fprintf(fp_write, "%d,%d,%d\n", SORTED_ARRAY[idx][0], SORTED_ARRAY[idx][1], SORTED_ARRAY[idx][2]);
	}
	fclose(fp_write);
	return SUCCEEDED;
}
//Description – This function creates all threads, mutex and semaphores
//Parameters – Four input arguments - the maximum number m may have, the number of threads, a path to the output file, and the size of the output buffer
//Returns – zero when successful
int create_threads_mutex_and_semaphores(int MAX_NUMBER, int threadnum, int *buffer_size)
{
	BOOL an_error_occured = FALSE;
	HANDLE *p_thread_handles = NULL;
	argu **Arguments = NULL;
	DWORD thread_id;
	int i;
	extern int *nums_for_calculation;
	

	extern HANDLE *SortSemaphore;
	extern char *outpath_file;
	
	int NUM_OF_THREADS = threadnum;
	

	//Create an array which contains the numbers for which Pythagorian triples are to be calculated
	nums_for_calculation = (int *)calloc(MAX_NUMBER, sizeof(int));
	if (NULL == nums_for_calculation)
	{
		
		ErrorPlan(outfile_path);
		exit(1);
	}

	
	
	Arguments = (argu **)malloc((NUM_OF_THREADS + 1) * sizeof(argu*));
	if (NULL == Arguments)
	{
		ErrorPlan(outfile_path);
		exit(1);
	}
	if (NULL == Arguments)
	{
		ErrorPlan(outfile_path);
		exit(1);
	}
	for (int q = 0; q < (NUM_OF_THREADS + 1); q++) {
		Arguments[q] = (argu *)malloc(sizeof(argu));
		if (NULL == Arguments[q])
		{
			ErrorPlan(outfile_path);
			exit(1);
		}
	}

	//Create an array of argu handles
	p_thread_handles = (argu *)malloc((NUM_OF_THREADS + 1) * sizeof(argu)); //NUM_OF_THREADS should later be replaced with the argument to the main of the same sort
	if (NULL == p_thread_handles)
	{
		ErrorPlan(outfile_path);
		exit(1);
	}



	//Create an array of mutexes that will enable each thread the reduction of the next number from the global array of numbers
	mutex_for_computations = (HANDLE *)malloc(MAX_NUMBER * sizeof(HANDLE)); //CHANGED!!!
	if (NULL==mutex_for_computations)
	{
		ErrorPlan(outfile_path);
		exit(1);
	}

	//Create threads and store them within the array p_thread_handles
	open_threads(NUM_OF_THREADS, &Arguments, &p_thread_handles, MAX_NUMBER, buffer_size);




	//Close handles and free allocated memory

	release_handles_and_free_space(&p_thread_handles, NUM_OF_THREADS, &Arguments, nums_for_calculation, buffer_size);

	return(0);
}
//Description – open threads all calculating threads and sorting thread
//Parameters – number of threads, array of arguments that need to be intialized and p_thread handles not intialized yet, max_number of arguments and the size of the buffer
//Returns – zero when successful
int open_threads(int NUM_OF_THREADS, argu ***Arguments, HANDLE **p_thread_handles, int MAX_NUMBER, int *buffer_size)
{
	extern char *outfile_path;
	extern HANDLE *mutex_for_computations;
	DWORD *thread_ids;
	DWORD wait_code;
	int i = 0;
	int idx;
	/* CHANGED */
	thread_ids = (DWORD *)malloc(NUM_OF_THREADS * sizeof(DWORD));
	if (NULL == thread_ids)
	{
		printf("Something wrong happened\n"); //We should change it to an error handling function
		return 1;
	}
	for (idx = 0; idx < MAX_NUMBER; idx++) {
		*(mutex_for_computations + idx) = CreateMutex(
			NULL,   /* default security attributes */
			FALSE,	/* don't lock mutex immediately */
			_T("%d", idx)	/* Set the mutex name to the specified index */
		);
	}
	/* CHANGED */

	while (i < NUM_OF_THREADS)
	{
	

		(*(*Arguments + i))->max_num = MAX_NUMBER;
		(*(*Arguments + i))->buffer_size = *buffer_size;
		(*(*Arguments + i))->mutex_index = SORTED_INIT_SIZE;
		(*p_thread_handles)[i] = CreateThreadSimple(Thread, *(*Arguments + i), (thread_ids + i), outfile_path);
		if ((*p_thread_handles)[i] == NULL) {
			printf("CreateThread %d failed.\n", nums_for_calculation[i]);
			return(1);
		}
		i++;
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	(*(*Arguments + i))->buffer_size = *buffer_size; //signals for sorting algorithm
	(*(*Arguments + i))->mutex_index = -1; //signals for sorting algorithm
	(*(*Arguments + i))->max_num = NUM_OF_THREADS; //signals for sorting algorithm

												   //After checking the creation of the primitive triples, we should add and check the sorting thread

	(*p_thread_handles)[i] = CreateThreadSimple(Thread, *(*Arguments + i), thread_ids + i, outfile_path); //open sorting thread

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	wait_code = WaitForSingleObject((*p_thread_handles)[i], INFINITE);
	if (wait_code != WAIT_OBJECT_0) {
		ErrorPlan(outfile_path);
		exit(1);
	}

	return(0);
}
//Description – releases all extra memory
//Parameters – handles of all threads array, number of threads, array of arguments structures, and buffer size
//Returns – zero when successful
int release_handles_and_free_space(HANDLE **p_thread_handles, int NUM_OF_THREADS, argu ***Arguments, int *buff_size)
{
	extern HANDLE *EmptySemaphore;
	extern HANDLE *FullSemaphore;
	extern HANDLE *mutex_for_computations;
	extern int *nums_for_calculation;
	extern HANDLE *mutex_for_buffer_idx;
	CloseHandle(EmptySemaphore);
	CloseHandle(FullSemaphore);
	extern int real_buff;
	
	int i;
	for (i = 0; i < real_buff; i++)
		CloseHandle(mutex_for_buffer_idx[i]);
	free(mutex_for_buffer_idx);
	int max_num = (**Arguments)->max_num;
	for (i = 0; i < max_num; i++) {
		CloseHandle(mutex_for_computations[i]);
	}
	for (i = 0; i < NUM_OF_THREADS + 1; i++)
		CloseHandle((*p_thread_handles)[i]);
	free(*p_thread_handles);
	free(mutex_for_computations);
	for (int q = 0; q < NUM_OF_THREADS + 1; q++)
		free((*Arguments)[q]);
	free(*Arguments);
	free(nums_for_calculation);
	//ErrorPlan("output.txt");
	return NULL;
}
//Description – intializes FullSem and EmptySem for producer consumer algorithm, which help in writing and reading to and from output_buffer
//Parameters – buffer size
//Returns – zero when successful
int Create_Read_and_Write_Semaphore(int buffer_size)
{
	
	
	extern HANDLE *EmptySemaphore;
	
	extern HANDLE *mutex_for_buffer_idx;
	int idx;
	//Create a semaphore for the Pythagorian triplets computation
	FullSemaphore = CreateSemaphore(
		NULL,				/* Default security attributes */
		0,		/* Initial the count to 0 */
		buffer_size,		/* Maximum Count */
		"Full_Sem"				/* un-named */
	);
	EmptySemaphore = CreateSemaphore(
		NULL,				/* Default security attributes */
		buffer_size,		/* Initial the count to 0 */
		buffer_size,		/* Maximum Count */
		"Empty_Sem"				/* un-named */
	);
	

	mutex_for_buffer_idx = (HANDLE *)malloc(buffer_size * sizeof(HANDLE));
	for (idx = 0; idx < buffer_size; idx++) {
		*(mutex_for_buffer_idx + idx) = CreateMutex(
			NULL,   /* default security attributes */
			FALSE,	/* don't lock mutex immediately */
			_T("%d", idx)	/* Set the mutex name to the specified index */
		);
	}
	return 0;
}
//Description - simple bubble sort
//Parameters - array of sorted elements, and size of elements inside.
//Return - returns 0 if succeeded
int BubbleSort(int ***ARRAY, int *s_size)
{
	int **ARR = *ARRAY;
	int i = 0, j = 0, m1 = 0, m2 = 0, n1 = 0, n2 = 0;

	for (i = 0; i < *s_size - 1; i++)
	{
		for (j = 0; j < *s_size - i - 1; j++)
		{
			n1 = ARR[j][4];
			n2 = ARR[j + 1][4];
			if (n1 > n2) {
				Swap(&ARR, &j);
			}
			else if (n1 == n2) {
				m1 = ARR[j][5];
				m2 = ARR[j + 1][5];
				if (m1 > m2) {
					Swap(&ARR, &j);
					m1 = ARR[j][5];
					m2 = ARR[j + 1][5];
				}
			}

		}
	}

	return 0;
}
int Swap(int ***ARRAY, int *j)
{
	int **ARR = *ARRAY;
	int *temp = NULL, i = 0;
	int a = -1, b = -1, t = -1;
	temp = (int *)calloc(6, sizeof(int));
	if (NULL == temp) {
	ErrorPlan(outfile_path); //could not allocate memory
	exit(1);
	}
	for (i = 0; i < 6; i++)
	{
		a = *(*(ARR + *j) + i);
		b = *(*(ARR + *j + 1) + i);
		t = *(temp + i);
		t = a;
		a = b;
		b = t;
		*(*(ARR + *j) + i) = a;
		*(*(ARR + *j + 1) + i) = b;

	}
	free(temp);
	return 0;
}

//Description - prints a double array of integers
//Parameters - array of sorted elements, and size of elements inside.
//Return - returns 0 if succeeded
int PrintArr(int ***ARR, int s_size)
{
	int counter = 0;
	for (int i = 0; i < s_size; i++)
	{
		if ((*ARR)[i][0] == 0) continue;
		printf("%d %d %d n:%d m:%d\n", (*ARR)[i][0], (*ARR)[i][1], (*ARR)[i][2], (*ARR)[i][4], (*ARR)[i][5]);
		counter++;
	}
	printf("The number of triples in sorted array is %d", counter);
	return 0;
}

int search_for_the_first_free_number(int max_num, int *min_n) 
{
	extern HANDLE *mutex_for_computations;//Used
	extern int **OUTPUT_BUFFER;
	extern int *nums_for_calculation;//used
	DWORD wait_res;
	int test_idx = 0; //check whether or not the array nums_for_calculation is empty
	int dummy_idx = 0;
	int n;
	for (dummy_idx = 0; dummy_idx < max_num; dummy_idx++) {
		test_idx = 0;
		wait_res = WaitForSingleObject(mutex_for_computations[dummy_idx], 0);
		
		if (0 == nums_for_calculation[dummy_idx])
		{
			test_idx = 1;
			if (WAIT_OBJECT_0 == wait_res)
			{
				nums_for_calculation[dummy_idx] = 1;
				ReleaseMutex(mutex_for_computations[dummy_idx]);
				return(dummy_idx + 1);
			}
			dummy_idx--;
		}
		if (WAIT_OBJECT_0 == wait_res)
		{
			ReleaseMutex(mutex_for_computations[dummy_idx]);
		}

	}

	return -1; //The array is empty
}

int find_gcd(int a, int b)
{
	int count, temp = 1;
	int min_num = min(a, b);
	int max_num = max(a, b);

	if (1 == min_num)
		return 1;
	for (count = 2; count <= min_num; count++)
	{
		if ((0 == (min_num % count)) && (0 == (max_num % count)))
			if (count > temp)
				temp = count;
	}
	return (temp);
}

int Choose_Index(int buff_size) 
{
	extern int **OUTPUT_BUFFER;
	extern HANDLE *mutex_for_buffer_idx;
	int idx;
	DWORD wait_res;
	DWORD release_res;
	for (idx = 0; idx < buff_size; idx++) {
		wait_res = WaitForSingleObject(mutex_for_buffer_idx[idx], 0);
		if (WAIT_OBJECT_0 == wait_res)
			if (1 == OUTPUT_BUFFER[idx][3])
				return (idx);
			else {
				release_res = ReleaseMutex(mutex_for_buffer_idx[idx]);
			}
		else {
			release_res = ReleaseMutex(mutex_for_buffer_idx[idx]);
		}
	}
	//printf("been there\n");
	return -1;
}