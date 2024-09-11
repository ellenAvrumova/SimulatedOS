# OS simulation
A simulated operating system made in C++.
You may clone this repo and enter the following line in the terminal to run it:

```
g++  -std=c++17  *.cpp  -o runme; ./runme
```

<strong>CPU scheduling</strong> is round-robin. Ready-queue is a real first-come-first-serve queue. Each process has a time limit for how long it can use the CPU at once. If a process uses the CPU for a longer time, it goes back to the end of the ready-queue.
 
For <strong>memory management</strong>, our simulation uses paging. If the memory is full, the least recently used frame is removed from memory. Your library must be able to simulate large amounts of memory (say 64 GB or even more). It is not allowed to represent each byte of memory individually. For example, it is not allowed to use vector with 1000 elements to represent 1000 bytes of memory.

<strong>Disk management</strong> is “first-come-first-served”. In other words, all disk I/O-queues are real queues (FIFO).

# Overview
Create a class <strong>SimOS</strong>. The following methods should be in it:
 
### SimOS( int numberOfDisks, unsigned long long amountOfRAM, unsigned int pageSize)
The parameters specify number of hard disks in the simulated computer, amount of memory, and page size.
Disks, frame, and page enumerations all start from 0.

### void NewProcess( )
Creates a new process in the simulated system. The new process takes place in the ready-queue or immediately starts using the CPU.
Every process in the simulated system has a PID. Your simulation assigns PIDs to new processes starting from 1 and increments it by one for each new process. Do not reuse PIDs of the terminated processes.

### void SimFork()       
The currently running process forks a child. The child is placed in the end of the ready-queue.

### void SimExit()
The process that is currently using the CPU terminates. Make sure you release the memory used by this process immediately. If its parent is already waiting, the process terminates immediately and the parent becomes runnable (goes to the ready-queue). If its parent hasn't called wait yet, the process turns into zombie.
To avoid the appearance of the orphans, the system implements the cascading termination. Cascading termination means that if a process terminates, all its descendants terminate with it.

### void SimWait()       
The process wants to pause and wait for any of its child processes to terminate. Once the wait is over, the process goes to the end of the ready-queue or the CPU. If the zombie-child already exists, the process proceeds right away (keeps using the CPU) and the zombie-child disappears. If more than one zombie-child exists, the system uses one of them (any!) to immediately resumes the parent, while other zombies keep waiting for the next wait from the parent.

### void TimerInterrupt()
Interrupt arrives from the timer signaling that the time slice of the currently running process is over.

### void DiskReadRequest( int diskNumber, std::string fileName )
Currently running process requests to read the specified file from the disk with a given number. The process issuing disk reading requests immediately stops using the CPU, even if the ready-queue is empty.

### void DiskJobCompleted( int diskNumber )
A disk with a specified number reports that a single job is completed. The served process should return to the ready-queue.
void AccessMemoryAddress(unsigned long long address)
Currently running process wants to access the specified logical memory address. System makes sure the corresponding page is loaded in the RAM. If the corresponding page is already in the RAM, its “recently used” information is updated.

### int GetCPU( )
GetCPU returns the PID of the process currently using the CPU. If CPU is idle it returns NO_PROCESS (see the supplied definitions above).

### std::deque<int> GetReadyQueue( )
GetReadyQueue returns the std::deque with PIDs of processes in the ready-queue where element in front corresponds start of the ready-queue.

### MemoryUsage GetMemory( )
GetMemory returns MemoryUsage vector describing all currently used frames of RAM. Remember, Terminated “zombie” processes don’t use memory, so they don’t contribute to memory usage.
MemoryItems appear in the MemoryUsage vector in the order they appear in memory (from low addresses to high).

### FileReadRequest GetDisk( int diskNumber )
GetDisk returns an object with PID of the process served by specified disk and the name of the file read for that process. If the disk is idle, GetDisk returns the default FileReadRequest object (with PID 0 and empty string in fileName) 

### std::deque<FileReadRequest> GetDiskQueue( int diskNumber )
GetDiskQueue returns the I/O-queue of the specified disk starting from the “next to be served” process.
 
If a disk with the requested number doesn’t exist throw `std::out_of_range` exception.
 
If instruction is called that requires a running process, but the CPU is idle, throw `std::logic_error` exception.

