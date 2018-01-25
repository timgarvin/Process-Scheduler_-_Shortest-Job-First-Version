# Description
This C++11 program simulates the scheduling of a set of k processes in a single processor system and also a multiprocessor system using the Shortest Job First (SJF) scheduling algorithm (the optimal algorithm choice for minimizing process waiting times). In this case, k = 50, but this program is also generalized enough to allow user input to define k instead. All that would be needed is the implementation of a user prompt. Each "process" is generated in a Doubly Linked List using a normal distribution function to randomize the cycles and memory footprints. Each node ("process") in the Doubly Linked List stores the process ID, the randomly generated number of CPU cycles, and the randomly generated memory footprint for each process. Each node ("process") in the Doubly Linked List also stores a pointer to the next node ("process") and the previous node ("process") in the Doubly Linked List of processes. I chose the Doubly Linked List as my data structure because it's about as simple as a Linked List, but is more optimized because you can go forwards or backwards through the Doubly Linked List. This helps to minimize the number of times you need to traverse through the Doubly Linked List of processes. Since the number of traversals through the Doubly Linked List are minimized, when compared to the more traditional Linked List data structure, this also should improve general runtime and actual execution time.

# Compilation and Execution
* Compile: g++ -std=c++0x 'process-scheduler-(sjf).cpp' -o 'process-scheduler-(sjf)'
* Run Program: ./'process-scheduler-(sjf)'

# Program Output
* This program prints output to the screen and also to two separate files: 'SJF Output (multi-processor).txt' and 'SJF Output (single processor).txt'.
	* The screen output lists the values for each process generated.
	* The file outputs lists each step taken during the process scheduling process and the resulting Average Waiting Times and Total Cycles.

# Technologies Used
* C++11
* Linux
* PuTTY
* WinSCP

# Notes
* This program should be compiled and executed in a Linux Operating System environment.