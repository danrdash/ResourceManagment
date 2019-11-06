# ResourceManagment
Used multiple threads inorder to calculate all primitive pythagorean triplets(https://en.wikipedia.org/wiki/Tree_of_primitive_Pythagorean_triples) given a positive natural integer
all pythagorean triples are calculated in parallel, a Producer-Consumer problem (https://en.wikipedia.org/wiki/Producer%E2%80%93consumer_problem) is solved using semaphores and mutexes inorder to create mutual exclusion.


## How to compile and run
make sure to insert the arguments, maximum number, numbe of threads, buffer size and the path to the output file as listed in the example below

### On Windows OS: 
1. download and install MinGW: https://osdn.net/projects/mingw/downloads/68260/mingw-get-setup.exe/
2. download all project files into the same folder, insert the following command: cd /...your_file_path.../
3. insert the following command: gcc *.c -o Pythagoras
4. insert the following command: Pythagoras max_number number_of_computation_threads output_buffer_size --output_file="/path_to_output_file/"

 * maximum number is for the natural postive integer you want to calculate the primitive pythagorean triples from
 * number of computation threads is trivial
 * buffer size should be larger than 10
 * output file path should be inserted




## IDE
Written and compiled in Visual Studio 2017

## License
[MIT](https://choosealicense.com/licenses/mit/)
