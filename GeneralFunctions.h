#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include "threads.h"

int print_to_file(char *path_to_file);
int create_threads_mutex_and_semaphores(int MAX_NUMBER, int threadnum, int *buffer_size);
int open_threads(int NUM_OF_THREADS, argu ***Arguments, HANDLE **p_thread_handles, int MAX_NUMBER, int *buffer_size);
int release_handles_and_free_space(HANDLE **p_thread_handles, int *NUM_OF_THREADS, argu ***Arguments, int *buff_size);
int Create_Read_and_Write_Semaphore(int buffer_size);
int BubbleSort(int ***ARRAY, int *s_size);
int Swap(int ***ARRAY, int *j);
int PrintArr(int ***ARR, int s_size);
int search_for_the_first_free_number(int max_num, int *min_n);
int find_gcd(int a, int b);
int Choose_Index(int buff_size);
int ErrorPlan(char *outpath);
int FinalRelease(int **SORTED_ARRAY, int **OUTPUT_BUFFER, int *buffer_size);
int InitMemory(int *buffer_size, char *output);
#endif