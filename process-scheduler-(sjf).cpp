//Tim Garvin

//Compile using: g++ -std=c++0x 'process-scheduler-(sjf).cpp' -o 'process-scheduler-(sjf)'
//Execute using: ./'process-scheduler-(sjf)'

/*Description: This C++11 program simulates the scheduling of a set of k processes using the Shortest Job First (SJF) scheduling algorithm (the optimal algorithm choice for minimizing process waiting times). In this 
			case, k = 50, but this program is also generalized enough to allow user input to define k instead. All that would be needed is the implementation of a user prompt. Each "process" is generated in a Doubly 
			Linked List using a normal distribution function to randomize the cycles and memory footprints. Each node ("process") in the Doubly Linked List stores the process ID, the randomly generated number of CPU 
			cycles, and the randomly generated memory footprint for each process. Each node ("process") in the Doubly Linked List also stores a pointer to the next node ("process") and the previous node ("process") 
			in the Doubly Linked List of processes. I chose the Doubly Linked List as my data structure because it's about as simple as a Linked List, but is more optimized because you can go forwards or backwards 
			through the Doubly Linked List. This helps to minimize the number of times you need to traverse through the Doubly Linked List of processes. Since the number of traversals through the Doubly Linked List 
			are minimized, when compared to the more traditional Linked List data structure, this also should improve general runtime and actual execution time.
*/

#include <iostream>
#include <random> //Required for normal_distribution function (C++11)
#include <iomanip> //Required for setw()
#include <algorithm> //Required for sort() library function
#include <vector> //Required for using vectors
#include <fstream> //Required for file input/output

using namespace std;

//Node structure for the Doubly Linked List of processes
struct node
{
	int process_ID;
	int CPU_cycles;
	int memory_footprint;
	int arrival_time;
	int start_time;
	int stop_time;
	int current_time;
	int previous_end_time;
	int waiting_time;
	int remaining_cycles;
	
	node *next_node;
	node *prev_node;
};

vector<node*> process_list; //Keeps an original, unchanged copy of the Doubly Linked List of processes
vector<node*> process_queue; //Used to queue processes for execution

int total_cycles = 0; //Keeps track of the total number of cycles amongst the whole set of processes

vector<node*> processor1_schedule; //Keeps track of the schedule for processor 1 in a multi-processor system
vector<node*> processor2_schedule; //Keeps track of the schedule for processor 2 in a multi-processor system
vector<node*> processor3_schedule; //Keeps track of the schedule for processor 3 in a multi-processor system
vector<node*> processor4_schedule; //Keeps track of the schedule for processor 4 in a multi-processor system

//Generates the number of processes specified by the user
node *generate_processes(node *head, int num_processes)
{
	node *current_node = head;
	node *previous_node = head;
	
	//For standard normal distribution: About 99.7% will be within +-3 standard deviations
	//So choose the correct standard deviation using: Standard Deviation = Midpoint/3 (where Midpoint = (lower bound + upper bound)/2
	
	//Cycles midpoint = (1,000 + 11,000)/2 = 6,000
	//Standard Deviation (cycles) = 6,000/3 = 2,000
	
	//Memory Footprints midpoint = (1 + 100)/2 = 50.50
	//Standard Deviation (memory footprints) = 50.5/3 = 16.8333 = 101/6
	
	//Declares variables for random number generation
	random_device rd;
	mt19937 randomNum_generator(rd());
	
	//Declares variables used to generate a random number using Normal Distribution for the number of CPU cycles and memory footprints for each process
	//normal_distribution<type> distribution(mean, standard deviation);
	normal_distribution<> cycles_distribution(6000, 2000);
	normal_distribution<> footprints_distribution(20, 101/6);
	
	//Declares and initializes variables to store the currently returned random number from the Normal Distribution random number generator for number of CPU cycles and memory footprints for each process
	int cycles = 0, footprint = 0;
	
	int arrival = 0; //Used to store and increment arrival time for each process
	
	//Prints headers for output
	cout<<endl;
	cout<<"Generated Processes: "<<endl<<endl;
	cout<<"Process ID\t"
	   <<"Cycles\t"
	   <<"Memory Footprint\t"
	   <<"Arrival Time\t"
	   <<"Remaining Time"<<endl;
	cout<<"------------------------------------------------------------------------------"<<endl;
	
	//Loop used to generate the number of processes in the Doubly Linked List that the user requests
	for(int i=0; i < num_processes; i++)
	{
		current_node->process_ID = i+1; //Sets the current process' process ID
		
		cycles = cycles_distribution(randomNum_generator); //Generates a random number for the number of CPU cycles for the current process
		
		//Checks if the currently generated random number for CPU cycles is between 1,000 - 11,000
		//If not between 1,000 - 11,000: Loop until a random number is generated that is
		while(cycles < 1000 || cycles > 11000)
		{
			cycles = cycles_distribution(randomNum_generator);
		}
		
		footprint = footprints_distribution(randomNum_generator); //Generates a random number for the memory footprint for the current process
		
		//Checks if the currently generated random number for the memory footprint is between 1 - 100
		//If not between 1 - 100: Loop until a random number is generated that is
		while(footprint < 1 || footprint > 100)
		{
			footprint = footprints_distribution(randomNum_generator);
		}
		
		//Stores the number of CPU cycles, memory footprint, and arrival time values generated for the current process
		current_node->CPU_cycles = cycles;
		current_node->memory_footprint = footprint;
		current_node->arrival_time = arrival;
		current_node->start_time = 0;
		current_node->stop_time = 0;
		current_node->current_time = 0;
		current_node->previous_end_time = 0;
		current_node->waiting_time = 0;
		current_node->remaining_cycles = cycles;
		
		//Prints the currently generated process' information
		cout<<setw(10)<<left<<"p" + to_string( current_node->process_ID )<<"\t"
			<<setw(6)<<right<<current_node->CPU_cycles<<"\t"
			<<setw(16)<<current_node->memory_footprint<<"\t"
			<<setw(12)<<current_node->arrival_time<<"\t"
			<<setw(14)<<current_node->remaining_cycles<<endl;
		
		//If not the first process generated: set the previous node value for the current process and move the previous_node pointer to the current node for the next process before iterating
		if(i > 0)
		{
			current_node->prev_node = previous_node;
			previous_node = current_node;
		}
		
		arrival += 50; //Increments the arrival time
		total_cycles += current_node->CPU_cycles;
		
		process_list[i] = current_node;
		
		//If not the last process generated: Create a new node to hold a new process and move the current_node pointer to that node
		if(i+1 < num_processes)
		{
			current_node->next_node = new node;
			current_node = current_node->next_node;
		}
	}
	
	cout<<endl<<endl;
	
	return current_node; //Return the tail of the Doubly Linked List of processes
}

//Resets all the values of the set of processes
void reset_processes(node *head, int num_processes)
{
	node *current_node = head; //Sets the starting node to the beginning of the Doubly Linked List
	
	//Loops through all of the processes in the Doubly Linked List, resetting all of their values
	for(int i = 0; i < num_processes; i++)
	{
		current_node->start_time = 0;
		current_node->stop_time = 0;
		current_node->previous_end_time = 0;
		current_node->waiting_time = 0;
		current_node->remaining_cycles = current_node->CPU_cycles;
		
		current_node = current_node->next_node;
	}
	
	delete current_node; //Deletes the current_node pointer
	
	//Clears out all of the processor schedules and the process queue
	processor1_schedule.clear();
	processor2_schedule.clear();
	processor3_schedule.clear();
	processor4_schedule.clear();
	process_queue.clear();
}

//Runs the Shortest Job First scheduling algorithm for the set of processes in a single processor system
void SJF_single_processor(node *head, int num_processes)
{
	ofstream sjf_file("SJF Output (single processor).txt"); //Declares an output file stream for Shortest Job First scheduling output

	//Runs the Shortest Job First scheduling algorithm for processes if the file has been opened without errors
	if (sjf_file.is_open())
	{
		
		process_queue = process_list; //Initializes the process queue to the process list (contains the whole Doubly Linked List of processes)
		
		//Sorts the process queue in ascending cycles order
		sort(process_queue.begin(), process_queue.end(), [](const node *a, const node *b) { 
			return a->CPU_cycles < b->CPU_cycles;
		});
		
		//Prints headers for output
		sjf_file<<"SJF (Shortest Job First) Schedule (Single Processor System):"<<endl<<endl;
		sjf_file<<"Process Order\t"
				<<"Cycles\t"
				<<"Memory Footprint\t"
				<<"Arrival Time\t"
				<<"Start Time\t"
				<<"Stop Time\t"
				<<"Waiting Time"<<endl;
		sjf_file<<"------------------------------------------------------------------------------------------------"<<endl;
		
		int current_time = 0, waiting_time = 0, total_waiting_time = 0;
		int i = 0;	
		
		//Loop through the process queue to execute each process until the process queue is empty
		while( !process_queue.empty() )
		{
			//If the arrival time for the current process has arrived: Execute the current process
			//Else: Check the next process in queue
			if(process_queue[i]->arrival_time <= current_time)
			{
				//Calculates the waiting time for the current process and increments it to the total waiting time overall
				waiting_time = current_time - process_queue[i]->arrival_time;
				total_waiting_time += waiting_time;
				
				//Prints the currently executing process' information
				sjf_file<<setw(13)<<left<<"p" + to_string( process_queue[i]->process_ID )<<"\t"
						<<setw(6)<<right<<process_queue[i]->CPU_cycles<<"\t"
						<<setw(16)<<process_queue[i]->memory_footprint<<"\t"
						<<setw(12)<<process_queue[i]->arrival_time<<"\t"
						<<setw(10)<<current_time<<"\t"; //Prints out the start time
				
				current_time += process_queue[i]->CPU_cycles; //Increments the current time to the stop time for the currently executing process
				
				sjf_file<<setw(10)<<current_time<<"\t" //Prints out the stop time
						<<setw(12)<<waiting_time<<endl;
				
				process_queue.erase(process_queue.begin()+i); //Removes the completely executed process from the process queue
				
				i = 0; //Goes back to the beginning of the process queue
			}
			else
			{
				i++; //Goes to the next process in the process queue
			}
		}
		
		sjf_file<<endl;
	
		double avg_waiting_time = 0;
		
		avg_waiting_time = total_waiting_time / num_processes;
		
		sjf_file<<"Average Waiting Time = "<<avg_waiting_time<<endl<<endl;  //Prints the average waiting time
		
		sjf_file<<"Total Cycles = "<<total_cycles;
	
		sjf_file.close(); //Closes the output file
		
		reset_processes(head, num_processes); //Resets all processes
	}
	else
	{
		cout<<"Unable to open the SJF (Shortest Job First) output file for a single processor system.";
	}
}

//Runs the Shortest Job First scheduling algorithm for the set of processes in a single processor system
void SJF_multiprocessor(node *head, int num_processes, int processor_count)
{
	ofstream sjf_file("SJF Output (multi-processor).txt"); //Declares an output file stream to write to a file for SJF (Shortest Job First) multi-processor scheduling output

	//Runs the SJF (Shortest Job First) scheduling algorithm for processes if the file to write to has been opened without errors
	if (sjf_file.is_open())
	{		
		process_queue = process_list; //Moves the Doubly Linked List of processes (copied to process_list previously) and creates a process queue
		
		//Sorts the process queue from shortest CPU cycles (shortest job) required to execute to the largest CPU Cycles required to execute
		sort(process_queue.begin(), process_queue.end(), [](const node *a, const node *b) { 
			return a->CPU_cycles < b->CPU_cycles;
		});
		
		//Prints Title
		sjf_file<<"SJF (Shortest Job First) Schedule (Multi-Processor System):"<<endl<<endl;
		
		int current_time = 0, total_waiting_time = 0; //Declares and initializes current_time (for keeping track of the current execution time) and total_waiting_time (for keeping track of the waiting time for all processes overall)

		int i = 0; //Declares and initializes a variable i (used for the current index in the process queue)
		
		int processor_cycles[4] = {0, 0, 0, 0}; //Declares and initializes an array of processor cycles (keeps track of REMAINING processor cycles)
		
		int min_cycles_index = 0; //Used for determining which index holds the smallest remaining time for the next time in the multi-processor schedule
		
		int processor1_index = 0, processor2_index = 0, processor3_index = 0, processor4_index = 0;  //Index variables to keep track of where the currently executing process is located in each processor's schedule
		
		
		//bool cycles_changed = false, 
		bool empty_processor = false, processes_left = true; //empty_processor: used initially to determine when a processor is empty (no executing processes), processes_left: keeps track of whether there are more processes that need to be executed or not
		
		int processes_executed = 0; //counts how many processes have finished executing so far
		
		//Loop used to generate an SJF (Shortest Job First) schedule for a multi-processor system
		while( processes_left )
		{
			//If the current process is ready to execute: execute the process in the process schedule
			//Else: Go to the next process in the process queue
			if(process_queue[i]->arrival_time <= current_time)
			{
				sjf_file<<"Current Time = "<<current_time<<endl; //Writes the current execution time to the output file
				
				//Writes headers to the output file
				sjf_file<<"Processor\t\t"
					  <<"Process ID\t"
					  <<"Cycles\t"
					  <<"Memory Footprint\t"
					  <<"Arrival Time\t"
					  <<"Start Time\t"
					  <<"Stop Time\t"
					  <<"Waiting Time\t"
					  <<"Remaining Cycles"<<endl;
				sjf_file<<"--------------------------------------------------------------------------------------------------------------------------------"<<endl;
				
				//If no processors are empty: Schedule the next process to execute
				//Else: Schedule the next process to execute in the next empty processor
				if(processor_cycles[min_cycles_index] > 0)
				{
					//If the process queue is NOT EMPTY: Remove the process that is done executing and schedule the next process to execute
					if(!process_queue.empty())
					{
						//Remove the process that is done executing and schedule the next process to execute, based on which process has the smallest cycles remaining
						switch(min_cycles_index)
						{
							case 0: //For Processor 1
							{
								processor1_schedule.push_back(process_queue[i]); //Add the new process to the processor schedule
								processor1_index++; //Point to the new process to execute
								
								process_queue.erase(process_queue.begin()+i); //Delete the finished process from the process queue
								
								processor1_schedule[processor1_index]->start_time = current_time; //Set the start time for the new process
								processor1_schedule[processor1_index]->stop_time = current_time + processor1_schedule[processor1_index]->CPU_cycles; //Calculate and set the stop time for the new process
								
								processor1_schedule[processor1_index]->waiting_time = current_time - processor1_schedule[processor1_index]->arrival_time; //Calculate and set the waiting time for the new process
								total_waiting_time += processor1_schedule[processor1_index]->waiting_time; //Increment the new waiting time into the total waiting time overall
							
								processor_cycles[0] = processor1_schedule[processor1_index]->CPU_cycles; //Initialize processor cycles
								
								break;
							}
							case 1: //For Processor 2
							{
								processor2_schedule.push_back(process_queue[i]); //Add the new process to the processor schedule
								processor2_index++; //Point to the new process to execute
							
								process_queue.erase(process_queue.begin()+i); //Delete the finished process from the process queue
								
								processor2_schedule[processor2_index]->start_time = current_time; //Set the start time for the new process
								processor2_schedule[processor2_index]->stop_time = current_time + processor2_schedule[processor2_index]->CPU_cycles; //Calculate and set the stop time for the new process
								
								processor2_schedule[processor2_index]->waiting_time = current_time - processor2_schedule[processor2_index]->arrival_time; //Calculate and set the waiting time for the new process
								total_waiting_time += processor2_schedule[processor2_index]->waiting_time; //Increment the new waiting time into the total waiting time overall
						
								processor_cycles[1] = processor2_schedule[processor2_index]->CPU_cycles; //Initialize processor cycles
								
								break;
							}
							case 2: //For Processor 3
							{
								processor3_schedule.push_back(process_queue[i]); //Add the new process to the processor schedule
								processor3_index++; //Point to the new process to execute
							
								process_queue.erase(process_queue.begin()+i); //Delete the finished process from the process queue
								
								processor3_schedule[processor3_index]->start_time = current_time; //Set the start time for the new process
								processor3_schedule[processor3_index]->stop_time = current_time + processor3_schedule[processor3_index]->CPU_cycles; //Calculate and set the stop time for the new process
								
								processor3_schedule[processor3_index]->waiting_time = current_time - processor3_schedule[processor3_index]->arrival_time; //Calculate and set the waiting time for the new process
								total_waiting_time += processor3_schedule[processor3_index]->waiting_time; //Increment the new waiting time into the total waiting time overall
						
								processor_cycles[2] = processor3_schedule[processor3_index]->CPU_cycles; //Initialize processor cycles
								
								break;
							}
							case 3: //For Processor 4
							{
								processor4_schedule.push_back(process_queue[i]); //Add the new process to the processor schedule
								processor4_index++; //Point to the new process to execute
							
								process_queue.erase(process_queue.begin()+i); //Delete the finished process from the process queue
								
								processor4_schedule[processor4_index]->start_time = current_time; //Set the start time for the new process
								processor4_schedule[processor4_index]->stop_time = current_time + processor4_schedule[processor4_index]->CPU_cycles; //Calculate and set the stop time for the new process
								
								processor4_schedule[processor4_index]->waiting_time = current_time - processor4_schedule[processor4_index]->arrival_time; //Calculate and set the waiting time for the new process
								total_waiting_time += processor4_schedule[processor4_index]->waiting_time; //Increment the new waiting time into the total waiting time overall
							
								processor_cycles[3] = processor4_schedule[processor4_index]->CPU_cycles; //Initialize processor cycles
								
								break;
							}
						}
					}
					
					//Loop through the processors and print out the current process execution information for each
					for(int index = 0; index < processor_count; index++)
					{
						//Print out the current processor's (based on index) current process execution information based on the current processor
						switch(index)
						{
							case 0: //For Processor 1
							{
								//Calculate and set the current remaining cycles left for the current process
								processor_cycles[0] = processor1_schedule[processor1_index]->stop_time - current_time;
								processor1_schedule[processor1_index]->remaining_cycles = processor_cycles[0];
								
								//If there are remaining cycles to execute: Print out current processes' information for the schedule
								//Else: The processor is currently empty (set empty_processor = true)
								if(processor_cycles[0] > 0)
								//if(processor1_index < processor1_schedule.size())
								{									
									//Prints the currently executing process' information
									sjf_file<<setw(12)<<left<<"Processor " + to_string(index+1) + ":"<<"\t"
										  <<setw(10)<<"p" + to_string( processor1_schedule[processor1_index]->process_ID )<<"\t"
										  <<setw(6)<<right<<processor1_schedule[processor1_index]->CPU_cycles<<"\t"
										  <<setw(16)<<processor1_schedule[processor1_index]->memory_footprint<<"\t"
										  <<setw(12)<<processor1_schedule[processor1_index]->arrival_time<<"\t"
										  <<setw(10)<<processor1_schedule[processor1_index]->start_time<<"\t" //Prints out the start time
										  <<setw(9)<<processor1_schedule[processor1_index]->stop_time<<"\t" //Prints out the stop time
										  <<setw(12)<<processor1_schedule[processor1_index]->waiting_time<<"\t" //Prints waiting time
										  <<setw(16)<<processor1_schedule[processor1_index]->remaining_cycles<<endl; //Prints remaining cycles
								}
								else
								{
									empty_processor = true;
								}
								
								break;
							}
							case 1: //For Processor 2
							{							
								//Calculate and set the current remaining cycles left for the current process
								processor_cycles[1] = processor2_schedule[processor2_index]->stop_time - current_time;
								processor2_schedule[processor2_index]->remaining_cycles = processor_cycles[1];
							
								//If there are remaining cycles to execute: Print out current processes' information for the schedule
								//Else: The processor is currently empty (set empty_processor = true)
								if(processor_cycles[1] > 0)
								{
									//Prints the currently executing process' information
									sjf_file<<setw(12)<<left<<"Processor " + to_string(index+1) + ":"<<"\t"
										  <<setw(10)<<"p" + to_string( processor2_schedule[processor2_index]->process_ID )<<"\t"
										  <<setw(6)<<right<<processor2_schedule[processor2_index]->CPU_cycles<<"\t"
										  <<setw(16)<<processor2_schedule[processor2_index]->memory_footprint<<"\t"
										  <<setw(12)<<processor2_schedule[processor2_index]->arrival_time<<"\t"
										  <<setw(10)<<processor2_schedule[processor2_index]->start_time<<"\t" //Prints out the start time
										  <<setw(9)<<processor2_schedule[processor2_index]->stop_time<<"\t" //Prints out the stop time
										  <<setw(12)<<processor2_schedule[processor2_index]->waiting_time<<"\t" //Prints waiting time
										  <<setw(16)<<processor2_schedule[processor2_index]->remaining_cycles<<endl; //Prints remaining cycles
								}
								else
								{
									empty_processor = true;
								}
								
								break;
							}
							case 2: //For Processor 3
							{
								//Calculate and set the current remaining cycles left for the current process
								processor_cycles[2] = processor3_schedule[processor3_index]->stop_time - current_time;
								processor3_schedule[processor3_index]->remaining_cycles = processor_cycles[2];
								
								//If there are remaining cycles to execute: Print out current processes' information for the schedule
								//Else: The processor is currently empty (set empty_processor = true)
								if(processor_cycles[2] > 0)
								{
									//Prints the currently executing process' information
									sjf_file<<setw(12)<<left<<"Processor " + to_string(index+1) + ":"<<"\t"
										  <<setw(10)<<"p" + to_string( processor3_schedule[processor3_index]->process_ID )<<"\t"
										  <<setw(6)<<right<<processor3_schedule[processor3_index]->CPU_cycles<<"\t"
										  <<setw(16)<<processor3_schedule[processor3_index]->memory_footprint<<"\t"
										  <<setw(12)<<processor3_schedule[processor3_index]->arrival_time<<"\t"
										  <<setw(10)<<processor3_schedule[processor3_index]->start_time<<"\t" //Prints out the start time
										  <<setw(9)<<processor3_schedule[processor3_index]->stop_time<<"\t" //Prints out the stop time
										  <<setw(12)<<processor3_schedule[processor3_index]->waiting_time<<"\t" //Prints waiting time
										  <<setw(16)<<processor3_schedule[processor3_index]->remaining_cycles<<endl; //Prints remaining cycles
								}
								else
								{
									empty_processor = true;
								}
								
								break;
							}
							case 3: //For Processor 4
							{
								//Calculate and set the current remaining cycles left for the current process
								processor_cycles[3] = processor4_schedule[processor4_index]->stop_time - current_time;
								processor4_schedule[processor4_index]->remaining_cycles = processor_cycles[3];
								
								//If there are remaining cycles to execute: Print out current processes' information for the schedule
								//Else: The processor is currently empty (set empty_processor = true)
								if(processor_cycles[3] > 0)
								{
									//Prints the currently executing process' information
									sjf_file<<setw(12)<<left<<"Processor " + to_string(index+1) + ":"<<"\t"
										  <<setw(10)<<"p" + to_string( processor4_schedule[processor4_index]->process_ID )<<"\t"
										  <<setw(6)<<right<<processor4_schedule[processor4_index]->CPU_cycles<<"\t"
										  <<setw(16)<<processor4_schedule[processor4_index]->memory_footprint<<"\t"
										  <<setw(12)<<processor4_schedule[processor4_index]->arrival_time<<"\t"
										  <<setw(10)<<processor4_schedule[processor4_index]->start_time<<"\t" //Prints out the start time
										  <<setw(9)<<processor4_schedule[processor4_index]->stop_time<<"\t" //Prints out the stop time
										  <<setw(12)<<processor4_schedule[processor4_index]->waiting_time<<"\t" //Prints waiting time
										  <<setw(16)<<processor4_schedule[processor4_index]->remaining_cycles<<endl; //Prints remaining cycles
								}
								else
								{
									empty_processor = true;
								}
								
								break;
							}
						}
						
						//If the current processor is empty: Print empty values (represented as - symbols)
						if(empty_processor)
						{
							//Prints the currently generated process' information
							sjf_file<<setw(12)<<left<<"Processor " + to_string(index+1) + ":"<<"\t"
								  <<setw(10)<<"-"<<"\t"
								  <<setw(6)<<right<<"-"<<"\t"
								  <<setw(16)<<"-"<<"\t"
								  <<setw(12)<<"-"<<"\t"
								  <<setw(10)<<"-"<<"\t"
								  <<setw(9)<<"-"<<"\t"
								  <<setw(12)<<"-"<<"\t"
								  <<setw(16)<<"-"<<endl;
							
							//If all processes have been executed: No more processes are left to execute (set processes_left = false)
							if(processes_executed == 50)
							{
								processes_left = false;
							}
							
							empty_processor = false; //Reset the empty processor status for the next processor
						}
					}
				
					sjf_file<<endl;
					
					//Initialize min_cycles_index in order to avoid assigning it to an empty (0 value) element in the array of remaining processor cycles
					for(int index=0; index < processor_count; index++)
					{
						//If the current element in the array of remaining processor cycles is non-zero: Set the min_cycles_index to the current index location in the array and exit the loop
						//Else: Increment the min_cycles_index location
						if(processor_cycles[index] > 0)
						{
							min_cycles_index = index;
							break;
						}
						else
						{
							min_cycles_index++;
						}
					}
					
					//Loop until the smallest remaining cycles left to execute for the currently executing processes is found
					for(int index=0; index < processor_count; index++)
					{
						//If the current element in the remaining processes cycle is non-zero and the current element is smaller than the previously set min_cycles_index: Set the new index location for the min_cycles_index
						if(processor_cycles[index] > 0 && processor_cycles[index] <= processor_cycles[min_cycles_index])
						{
							min_cycles_index = index;
						}
					}
					
					processes_executed++; //Increment the number of processes that have completed execution
					
					current_time += processor_cycles[min_cycles_index]; //Increment the current time based upon the process with the smallest cycles left to execute (this is the next time the processor would swap processes)
				}
				else //Currently empty processor(s)
				{
					//Schedule the first process to execute in processor 1, based on which process has the smallest cycles remaining
					switch(min_cycles_index)
					{
						case 0: //For Processor 1
						{
							processor1_schedule.push_back(process_queue[i]); //Add the new process to the processor schedule
					
							process_queue.erase(process_queue.begin()+i); //Delete the new process from the process queue
							
							processor1_schedule[0]->start_time = current_time; //Set the start time for the new process
							processor1_schedule[0]->stop_time = processor1_schedule[0]->CPU_cycles; //Calculate and set the stop time for the new process
							
							processor1_schedule[0]->waiting_time = current_time - processor1_schedule[0]->arrival_time; //Calculate and set the waiting time for the new process
							total_waiting_time += processor1_schedule[processor1_index]->waiting_time; //Increment the new waiting time into the total waiting time overall
							
							processor_cycles[0] = processor1_schedule[0]->CPU_cycles; //Initialize processor cycles
							
							break;
						}
						case 1: //For Processor 2
						{
							processor2_schedule.push_back(process_queue[i]); //Add the new process to the processor schedule
						
							process_queue.erase(process_queue.begin()+i); //Delete the new process from the process queue
							
							processor2_schedule[0]->start_time = current_time; //Set the start time for the new process
							processor2_schedule[0]->stop_time = current_time + processor2_schedule[0]->CPU_cycles; //Calculate and set the stop time for the new process
							
							processor2_schedule[0]->waiting_time = current_time - processor2_schedule[0]->arrival_time; //Calculate and set the waiting time for the new process
							total_waiting_time += processor2_schedule[processor2_index]->waiting_time; //Increment the new waiting time into the total waiting time overall
							
							processor_cycles[1] = processor2_schedule[0]->CPU_cycles; //Initialize processor cycles
							
							break;
						}
						case 2: //For Processor 3
						{
							processor3_schedule.push_back(process_queue[i]); //Add the new process to the processor schedule
						
							process_queue.erase(process_queue.begin()+i); //Delete the new process from the process queue
							
							processor3_schedule[0]->start_time = current_time; //Set the start time for the new process
							processor3_schedule[0]->stop_time = current_time + processor3_schedule[0]->CPU_cycles; //Calculate and set the stop time for the new process
							
							processor3_schedule[0]->waiting_time = current_time - processor3_schedule[0]->arrival_time; //Calculate and set the waiting time for the new process
							total_waiting_time += processor3_schedule[processor3_index]->waiting_time; //Increment the new waiting time into the total waiting time overall
					
							processor_cycles[2] = processor3_schedule[0]->CPU_cycles; //Initialize processor cycles
							
							break;
						}
						case 3: //For Processor 4
						{
							processor4_schedule.push_back(process_queue[i]); //Add the new process to the processor schedule
						
							process_queue.erase(process_queue.begin()+i); //Delete the new process from the process queue
							
							processor4_schedule[0]->start_time = current_time; //Set the start time for the new process
							processor4_schedule[0]->stop_time = current_time + processor4_schedule[0]->CPU_cycles; //Calculate and set the stop time for the new process
							
							processor4_schedule[0]->waiting_time = current_time - processor4_schedule[0]->arrival_time; //Calculate and set the waiting time for the new process
							total_waiting_time += processor4_schedule[processor4_index]->waiting_time; //Increment the new waiting time into the total waiting time overall
						
							processor_cycles[3] = processor4_schedule[0]->CPU_cycles; //Initialize processor cycles
							
							processes_executed++; //Increment the number of processes that have completed execution
							
							break;
						}
					}
					
					//Loop through the processors and print out the current process execution information for each
					for(int index = 0; index < processor_count; index++)
					{
						if(index > min_cycles_index) //Print out currently empty processors
						{
							//Prints the currently generated process' information
							sjf_file<<setw(12)<<left<<"Processor " + to_string(index+1) + ":"<<"\t"
								  <<setw(10)<<"-"<<"\t"
								  <<setw(6)<<right<<"-"<<"\t"
								  <<setw(16)<<"-"<<"\t"
								  <<setw(12)<<"-"<<"\t"
								  <<setw(10)<<"-"<<"\t"
								  <<setw(9)<<"-"<<"\t"
								  <<setw(12)<<"-"<<"\t"
								  <<setw(16)<<"-"<<endl;
						}
						else //Print out currently running processors
						{
							//Print out the current processor's (based on index) current process execution information based on the current processor
							switch(index)
							{
								case 0: //For Processor 1
								{
									//If the current time is not 0: Decrement remaining cycles for the process in Processor 1
									if(current_time > 0)
									{
										processor1_schedule[0]->remaining_cycles -= 50;
										processor_cycles[0] -= 50;
									}
									
									//Prints the currently executing process' information
									sjf_file<<setw(12)<<left<<"Processor " + to_string(index+1) + ":"<<"\t"
										  <<setw(10)<<"p" + to_string( processor1_schedule[0]->process_ID )<<"\t"
										  <<setw(6)<<right<<processor1_schedule[0]->CPU_cycles<<"\t"
										  <<setw(16)<<processor1_schedule[0]->memory_footprint<<"\t"
										  <<setw(12)<<processor1_schedule[0]->arrival_time<<"\t"
										  <<setw(10)<<processor1_schedule[0]->start_time<<"\t" //Prints out the start time
										  <<setw(9)<<processor1_schedule[0]->stop_time<<"\t" //Prints out the stop time
										  <<setw(12)<<processor1_schedule[0]->waiting_time<<"\t" //Prints waiting time
										  <<setw(16)<<processor1_schedule[0]->remaining_cycles<<endl; //Prints remaining cycles
									break;
								}
								case 1: //For Processor 2
								{
									//If the current time is not 50: Decrement remaining cycles for the process in Processor 1
									if(current_time > 50)
									{
										processor2_schedule[0]->remaining_cycles -= 50;
										processor_cycles[1] -= 50;
									}
										
									//Prints the currently executing process' information
									sjf_file<<setw(12)<<left<<"Processor " + to_string(index+1) + ":"<<"\t"
										  <<setw(10)<<"p" + to_string( processor2_schedule[0]->process_ID )<<"\t"
										  <<setw(6)<<right<<processor2_schedule[0]->CPU_cycles<<"\t"
										  <<setw(16)<<processor2_schedule[0]->memory_footprint<<"\t"
										  <<setw(12)<<processor2_schedule[0]->arrival_time<<"\t"
										  <<setw(10)<<processor2_schedule[0]->start_time<<"\t" //Prints out the start time
										  <<setw(9)<<processor2_schedule[0]->stop_time<<"\t" //Prints out the stop time
										  <<setw(12)<<processor2_schedule[0]->waiting_time<<"\t" //Prints waiting time
										  <<setw(16)<<processor2_schedule[0]->remaining_cycles<<endl; //Prints remaining cycles
									
									break;
								}
								case 2: //For Processor 3
								{
									//If the current time is not 100: Decrement remaining cycles for the process in Processor 1
									if(current_time > 100)
									{
										processor3_schedule[0]->remaining_cycles -= 50;
										processor_cycles[2] -= 50;
									}
									
									//Prints the currently executing process' information
									sjf_file<<setw(12)<<left<<"Processor " + to_string(index+1) + ":"<<"\t"
										  <<setw(10)<<"p" + to_string( processor3_schedule[0]->process_ID )<<"\t"
										  <<setw(6)<<right<<processor3_schedule[0]->CPU_cycles<<"\t"
										  <<setw(16)<<processor3_schedule[0]->memory_footprint<<"\t"
										  <<setw(12)<<processor3_schedule[0]->arrival_time<<"\t"
										  <<setw(10)<<processor3_schedule[0]->start_time<<"\t" //Prints out the start time
										  <<setw(9)<<processor3_schedule[0]->stop_time<<"\t" //Prints out the stop time
										  <<setw(12)<<processor3_schedule[0]->waiting_time<<"\t" //Prints waiting time
										  <<setw(16)<<processor3_schedule[0]->remaining_cycles<<endl; //Prints remaining cycles
									
									break;
								}
								case 3: //For Processor 4
								{
									//Prints the currently executing process' information
									sjf_file<<setw(12)<<left<<"Processor " + to_string(index+1) + ":"<<"\t"
										  <<setw(10)<<"p" + to_string( processor4_schedule[0]->process_ID )<<"\t"
										  <<setw(6)<<right<<processor4_schedule[0]->CPU_cycles<<"\t"
										  <<setw(16)<<processor4_schedule[0]->memory_footprint<<"\t"
										  <<setw(12)<<processor4_schedule[0]->arrival_time<<"\t"
										  <<setw(10)<<processor4_schedule[0]->start_time<<"\t" //Prints out the start time
										  <<setw(9)<<processor4_schedule[0]->stop_time<<"\t" //Prints out the stop time
										  <<setw(12)<<processor4_schedule[0]->waiting_time<<"\t" //Prints waiting time
										  <<setw(16)<<processor4_schedule[0]->remaining_cycles<<endl; //Prints remaining cycles
									
									break;
								}
							}
						}
					}
					
					sjf_file<<endl;
					
					//If not all processors are executing processes: Increment to the next processor and new time
					//Else: Determine the process with the current smallest remaining cycles so it can be swapped with a new process
					if(min_cycles_index < processor_count-1)
					{
						min_cycles_index++;
					
						current_time += 50;	
					}
					else
					{
						min_cycles_index = 0;
						
						//Loop to determine the index with the smallest remaining cycles to execute
						for(int index=0; index < processor_count; index++)
						{
							if(processor_cycles[index] < processor_cycles[min_cycles_index])
							{
								min_cycles_index = index;
							}
						}
						
						current_time += processor_cycles[min_cycles_index]; //Increment to the next current time (when a new process will be swapped for a currently executing process)
					}
				}
				
				i = 0; //Reset the process queue index back to the beginning before determining the next process to execute
			}
			else
			{
				i++; //Increment the process queue index back to look at the next possible process to execut
			}
		}
		
		sjf_file<<endl;
		
		double avg_waiting_time = 0;
		
		avg_waiting_time = total_waiting_time / num_processes;
		
		sjf_file<<"Average Waiting Time = "<<avg_waiting_time;
	
		sjf_file.close();
		
		reset_processes(head, num_processes);
	}
	else
	{
		cout<<"Unable to open the SJF (Shortest Job First) output file for a multi-processor system.";
	}
}

int main()
{
	int num_processes = 50, processor_count = 4; //Sets the number of processes to generate to 50
	
	int quantum = 50, context_switch_penalty = 10; //Sets the quantum to 50 and context switch penalty to 10
	
	process_list.resize(num_processes); //Resizes the process list to the size that the Doubly Linked List of processes will be
	
	node *head = new node; //Declares and initializes the head node pointer of the Doubly Linked List of processes
	node *tail; //Declares the pointer to the tail of the Doubly Linked List of processes
	
	tail = generate_processes(head, num_processes); //Generates the number of processes requested by the user and returns the tail of the Doubly Linked List of processes once after the last process has been generated
	
	//Run the Shortest Job First (SJF) process scheduling algorithm for a single processor system
	SJF_single_processor(head, num_processes);
	
	//Run the Shortest Job First (SJF) process scheduling algorithm for a multi-processor system
	SJF_multiprocessor(head, num_processes, processor_count);
	
	//Deletes the head and tail pointers for the Doubly Linked List of processes before exiting the program
	delete head;
	delete tail;
	
	return 0;
}