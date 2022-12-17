# Edge-Computing-Linux-Scheduler-Policies
Task 4 for Part 2 of the Interview Process for the Edge Computing Position at Chalmers University of Technology

The C++ files are the single-threaded and multi-threaded implementations of the following four algorithms:

- Generating uniformly distributed random integers
- Randomly shuffling a container of string tokens
- Determining the mean, variance, standard deviation, minimum, and maximum of uniformly distributed random integers
- Computing all of the possible permutations of a heterogram

The ```Run...''' Python files are for testing the multi-threaded C++ programs for the following Linux scheduler policies:


- ```SCHED_OTHER''
- ```SCHED_FIFO'''
- ```SCHED_RR'''
- ```SCHED_BATCH'''
- ```SCHED_IDLE'''

and for producing txt files, from which the ```run_...''' Python files produce plots for the run-times.



To compile the single threaded C++ Programs, execute the following:

- g++ -o file file.cpp -std=c++17


To compile the multi-threaded C++ Programs, execute the following:

- g++ -o file file.cpp -std=c++17 -lpthread


Once the multi-threaded file is compiled, one can run the corresponding ```Run...''' and ```run_...''' files.
