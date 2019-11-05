//Description –this module consists of library includes definitions of important integers and function declarations for threads.

#ifndef THREADS_H
#define THREADS_H
#include <windows.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <tchar.h>



#define FAILED 1
#define SUCCEEDED 0
#define SORTED_INIT_SIZE 210000
#define MAX_LINE 100
#define NUM_THREADS 2
#define BRUTAL_TERMINATION_CODE 0x55
#define ERROR_CODE ((int)(-1))
#define MAX_ARG 100
#define MAX_OUTPATH 50
#define MAX_TESTNAME 100
#define MIN_TIMEOUT_FOR_SORT 100
#pragma warning(disable:4996)

typedef struct Arguments {
	int *first_num; // The first number for which the algorithm calculates triplets
	int max_num; // The maximal number
	int mutex_index; // Set the index in the array of mutexes as each mutex's identifier
	int buffer_size;
	DWORD exitc;
} argu;
argu **threads_for_routine; //This array of structs is meant for storing the thread structs in order

// Function Declarations -------------------------------------------------------
int SortAlg(argu** argum);
BOOL Pythagorean_triples(int max_num, int mutex_index, int buff_size);
DWORD WINAPI Thread(LPVOID lpParam);
HANDLE CreateThreadSimple(LPTHREAD_START_ROUTINE p_start_routine, LPVOID arguments, LPDWORD p_thread_id, char *file_outpath);

#endif