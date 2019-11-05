//Description –this module consists of functions that create threads.

#include "threads.h"
#include "GeneralFunctions.h"

//Description - when a thread opens, it chooses wether to enter a function that calculates primite pythagorean triple OR enters into SortAlg
//Parameters - LPParam, we chose to enter an argu strcture, and cast it inside
//returns 0 if succeded
DWORD WINAPI Thread(LPVOID lpParam)
{
	argu *thread = NULL;

	if (NULL == lpParam)
		return ERROR_CODE;
	thread = (argu *)lpParam;
	if (thread->mutex_index == -1)
	{
		SortAlg(&thread);
	}
	else {
		Pythagorean_triples(thread->max_num, thread->mutex_index, thread->buffer_size);
	}
	return(0);
}
//Description - algorithm is a loop, in each loop we act as a consumer in a producer consumer problem. we take FullSemaphore down, take hold of a mutex that's available
//copy from buffer into a sorting_array, release mutex and EmptySemaphore up. we do this until all threads are finished and the buffer is empty.
//Parameters - argu strcuture that contains all parameters we need
//returns 0 if succeeded
int SortAlg(argu** argum)
{
	
	extern  HANDLE *mutex_for_buffer_idx, *FullSemaphore, *EmptySemaphore;
	extern int next_up;
	extern HANDLE semaphores_for_computations;
	extern int **OUTPUT_BUFFER;
	extern int **SORTED_ARRAY;
	extern int thread_tag;
	argu *args = *argum;
	DWORD wait_res1;
	DWORD wait_res2;
	BOOL release_res1=-1;
	BOOL release_res2=-1;
	BOOL release_res3 = -1;
	extern valid_bit, thread_tag;
	int *buffer_size = NULL, i = 0, j = 0, buff_index = 0, sort_index = 0, runner = 0, ovf = 0, flag = 0, s_size=SORTED_INIT_SIZE;
	LPLONG prev = 0;
	buffer_size = &(args->buffer_size);
	//while (valid_bit < (args->max_num)) //signals wether the threads have finished writing to OUTPUT_BUFFER
	while (1)
	{
		
		if ((valid_bit == 0) && (thread_tag == args->max_num))
			break;
		if (runner == *buffer_size) // if we reached end of buffer, reset index
			runner = 0;
		if (1 == OUTPUT_BUFFER[runner][3]) {
			if (next_up != 0)
				runner = next_up;
			else
				runner++;
			continue;
		}
		wait_res1 = WaitForSingleObject(FullSemaphore, INFINITE); //semaphore down
		if (wait_res1 != 0) {
			printf("Consumer: FullSemaphore up count failed because, %d\n", GetLastError());
		}
		wait_res1 = WaitForSingleObject(mutex_for_buffer_idx[runner], 0);
		if (wait_res1 == WAIT_TIMEOUT) {

			release_res1 = ReleaseMutex(mutex_for_buffer_idx[runner]);
			if (next_up != 0)
				runner = next_up;
			else
				runner++;
			if (0 == release_res1) {
				//printf("%d", GetLastError());
			}
			release_res2 = ReleaseSemaphore(FullSemaphore, 1, NULL); //semaphore release
			if (release_res2 == 0)
				printf("Consumer: Full Semaphore release failed because, %d\n", GetLastError());
			continue;
		}

		if ((WAIT_OBJECT_0 == wait_res1)) // check if mutex is locked or not, if flag is off transfer to buffer
		{
			for (i = 0; i < 6; i++) {
				if (i == 3) continue;
				SORTED_ARRAY[sort_index][i] = OUTPUT_BUFFER[runner][i];
			}
			OUTPUT_BUFFER[runner][3] = 1;
			sort_index++;
			valid_bit--;
			next_up = 0;
			OUTPUT_BUFFER[runner][3] = 1; // signaled wether i transfered or not. 
			release_res1 = ReleaseMutex(mutex_for_buffer_idx[runner]); //release mutex
			if (0 == release_res1)
				printf("%d\n", GetLastError());
			release_res2 = ReleaseSemaphore(EmptySemaphore, 1, NULL); //empty semaphore increase by 1
			if (release_res2 == 0)
				printf("Consumer: empty Semaphore release failed because, %d\n", GetLastError());
			runner++;
		}
	
	}
	sort_index++;
	BubbleSort(&SORTED_ARRAY, &(sort_index));
	
	return 0;

}
//Description – this function calculates all primitive pythagorean triples up to m = MAX_NUMBER
//Parameters – Three input arguments - the maximal number m may get (MAX_NUMBER), an identifier to the mutex, and the size of OUTPUT_BUFFER
//Returns – returns TRUE when successful
BOOL Pythagorean_triples(int max_num, int mutex_index, int buff_size)
{

	int m, n, a, b, c;
	static int number = 0;
	static int idx = 0;
	extern HANDLE *mutex_for_computations;//Used
	extern int **OUTPUT_BUFFER, next_up;
	extern int *nums_for_calculation;
	extern int valid_bit, thread_tag;
	extern  HANDLE *mutex_for_buffer_idx, *FullSemaphore, *EmptySemaphore;
	int initial_state;
	//int mutex_idx;
	int dummy_idx = 0;

	DWORD wait_res1;
	DWORD wait_res2;
	BOOL release_res;
	BOOL release_res2;

	static int mutex_idx = 0;
	static int min_n = 0;

	/* Calculate all the Pythagorian triples defined by max_num */
	while (number < max_num)
	{
		n = search_for_the_first_free_number(max_num, &min_n);
		if (-1 == n)// The mutex for the value of n is taken
		{
			break;
		}
	
		for (m = n + 1; m <= max_num; m += 2)
		{
			if (1 == find_gcd(n, m))
			{
				wait_res1 = WaitForSingleObject(EmptySemaphore, 1000); //semaphore down
				if (wait_res1 != 0) {
					//printf("Producer: EmptySemaphore up count failed becauseA, %d\n", GetLastError());
				}
				dummy_idx = Choose_Index(buff_size);
				if (-1 == dummy_idx) {
					m -= 2;
					continue;
				}
			
				/* Write to the buffer */
				OUTPUT_BUFFER[dummy_idx][3] = 0;
				OUTPUT_BUFFER[dummy_idx][0] = abs(n * n - m * m);
				OUTPUT_BUFFER[dummy_idx][1] = abs(2 * m * n);
				OUTPUT_BUFFER[dummy_idx][2] = abs(m * m + n * n);
				OUTPUT_BUFFER[dummy_idx][4] = n;
				OUTPUT_BUFFER[dummy_idx][5] = m;
				valid_bit++;
				next_up = dummy_idx;
				/* Get out of the critical section */
				release_res = ReleaseMutex(mutex_for_buffer_idx[dummy_idx]);
				if (release_res == 0)
					printf("buffer mutex failed to be released because, %d", GetLastError());
				release_res2 = ReleaseSemaphore(FullSemaphore, 1, NULL); //full up 
				if (release_res2 == 0)
					printf("Producer: Full Semaphore release failed because, %d\n", GetLastError());
			}
		}
		release_res = ReleaseMutex(mutex_for_computations[n - 1]);
		number++;
		
	}
	thread_tag++;
	return(TRUE);
}
//Description - creates a thread, taken from class notes
//Parameters - p_start routine, arguments p_thread_id, file_outpath
//returns thread handle
HANDLE CreateThreadSimple(LPTHREAD_START_ROUTINE p_start_routine, LPVOID arguments, LPDWORD p_thread_id, char *file_outpath)
{ // arguments represent the arguments of the thread's routine
	extern char outfile_path;
	HANDLE thread_handle;
	if (NULL == p_start_routine)
	{
		printf("Error when creating a thread");
		printf("Received null pointer");
		ErrorPlan(outfile_path);
		exit(ERROR_CODE);
	}

	if (NULL == p_thread_id)
	{
		printf("Error when creating a thread");
		printf("Received null pointer");
		ErrorPlan(outfile_path);
		exit(ERROR_CODE);
	}
	thread_handle = CreateThread(
		NULL,            /*  default security attributes */
		0,               /*  use default stack size */
		p_start_routine, /*  thread function */
		arguments,            /*  argument to thread function */
		0,               /*  use default creation flags */
		p_thread_id);    /*  returns the thread identifier */

	if (NULL == thread_handle)
	{
		printf("Couldn't create thread\n");
		ErrorPlan(outfile_path);
		exit(ERROR_CODE);
	}

	return thread_handle;
}