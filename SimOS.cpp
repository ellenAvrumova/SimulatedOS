// Ellen Avrumova

#include <iostream>
#include "SimOS.h"

SimOS::SimOS(int numberOfDisks, unsigned long long amountOfRAM, unsigned int pageSize)
{
    this->numberOfDisks = numberOfDisks;
    DiskManager tempDisks(numberOfDisks);
    this->disks = tempDisks;
    this->amountOfRAM = amountOfRAM;
    this->pageSize = pageSize;
    unsigned long long numFrames = amountOfRAM/pageSize;
    MemoryManager tempRAM(numFrames);
    this->ram = tempRAM;
    this->previousPID = 1;
}

// Creates a new process in the simulated system. The new process takes place in the ready-queue or immediately starts using the CPU.
// Every process in the simulated system has a PID.
// Your simulation assigns PIDs to new processes starting from 1 and increments it by one for each new process.
// Do not reuse PIDs of the terminated processes.
void SimOS::NewProcess()
{
    ProcessManager process(previousPID);
    previousPID++;
    
    this->cpu.AddToReadyQueue(process);
    processMap.insert({process.getPID(), process});
}

// The currently running process forks a child. The child is placed in the end of the ready-queue.
void SimOS::SimFork()
{
    if (this->cpu.getCurrentProcess().getPID() == NO_PROCESS) {
        throw std::logic_error("CPU IS IDLE.");
    }
    ProcessManager& parent = this->cpu.getCurrentProcess();
    ProcessManager child(previousPID);
    previousPID++;

    child.setParent(parent.getPID());
    parent.addChild(child.getPID());

    this->cpu.AddToReadyQueue(child);

    processMap.insert({child.getPID(), child});
    processMap[parent.getPID()] = parent; // need to update the parent w/ new child info process in the map
}

// The process that is currently using the CPU terminates.
// Make sure you release the memory used by this process immediately.
// If its parent is already waiting, the process terminates immediately and the parent becomes runnable (goes to the ready-queue).
// If its parent hasn't called wait yet, the process turns into zombie.
// To avoid the appearance of the orphans, the system implements the cascading termination.
// Cascading termination means that if a process terminates, all its descendants terminate with it.
void SimOS::SimExit()
{
    if (this->cpu.getCurrentProcess().getPID() == NO_PROCESS) {
        throw std::logic_error("CPU IS IDLE.");
    }
    // Check if the parent is waiting.
    int parentPID = this->cpu.getCurrentProcess().getParentPID();
    this->ram.removeFromMemory(this->cpu.getCurrentProcess().getPID()); // remove current process from memory
    ProcessManager parent;
    bool waiting = false;
    for(int i = 0; i < waitList.size(); i++) {
        if(parentPID == waitList[i].getPID()) {
            parent = waitList[i];
            waiting = true;
            break;
        }
    }
    if(waiting) {
        destroyChildren(this->cpu.getCurrentProcess());
        cpu.AddToReadyQueue(parent);
        cpu.stopUsingCPU();
    }
    else {
        zombieList.push_back(this->cpu.getCurrentProcess());
        destroyChildren(this->cpu.getCurrentProcess());
        cpu.stopUsingCPU();
    }
}

void SimOS::destroyChildren(ProcessManager& pcb) {
    for(int child : pcb.getChildrenList()) {
        ProcessManager childProcess = processMap[child];
        this->ram.removeFromMemory(childProcess.getPID());

        // processes can either be in Ready Queue or IO Queue
        // if it's not in one, it must be in the other
        if(this->cpu.removeFromReadyQueue(childProcess.getPID())) {
            destroyChildren(childProcess);
        }
        else {
            for(Disk& d : this->disks.getDiskList()) {
                d.removeFromIOQueue(childProcess.getPID());
                destroyChildren(childProcess);
            }
        }
    }
}

// The process wants to pause and wait for any of its child processes to terminate.
// Once the wait is over, the process goes to the end of the ready-queue or the CPU.
// If the zombie-child already exists, the process proceeds right away (keeps using the CPU) and the zombie-child disappears.
// If more than one zombie-child exists, the system uses one of them (any!) to immediately resumes the parent,
// while other zombies keep waiting for the next wait from the parent.
void SimOS::SimWait()
{
    if (this->cpu.getCurrentProcess().getPID() == NO_PROCESS) {
        throw std::logic_error("CPU IS IDLE.");
    }
    for(int i = 0; i < this->cpu.getCurrentProcess().getChildrenList().size(); i++) {
        for(int j = 0; j < zombieList.size(); j++) {
            if(this->cpu.getCurrentProcess().getChildrenList()[i] == zombieList[j].getPID()) {
                // erase from zombie list
                auto iterator = zombieList.begin() + j;
                zombieList.erase(iterator);
                this->cpu.getCurrentProcess().removeChild(i);
                return;
            }
        }
    }
    waitList.push_back(this->cpu.getCurrentProcess()); // if a zombie wasn't found, put the process on the wait list
    this->cpu.stopUsingCPU(); // then next process begins execution
}

// Interrupt arrives from the timer signaling that the time slice of the currently running process is over.
void SimOS::TimerInterrupt()
{
    this->cpu.timerInterrupt();
}

void SimOS::DiskReadRequest(int diskNumber, std::string fileName)
{
    if(diskNumber >= numberOfDisks || diskNumber < 0) {
        throw std::out_of_range("DISK NUMBER ISN'T VALID.");
    }
    if (this->cpu.getCurrentProcess().getPID() == 0) {
        throw std::logic_error("CPU IS IDLE.");
    }
    else {
        this->disks.getDisk(diskNumber).readRequest(this->cpu.getCurrentProcess(), fileName);
        this->cpu.stopUsingCPU();
    }
}

void SimOS::DiskJobCompleted(int diskNumber)
{
    if(diskNumber >= numberOfDisks || diskNumber < 0) {
        throw std::out_of_range("DISK NUMBER ISN'T VALID.");
    }
    ProcessManager currentProcess = this->disks.getDisk(diskNumber).getCurrentProcess(); // save the PID of the current process getting help from the disk
    if(currentProcess.getPID() == 0) { // don't add an empty process to the ready queue
        return;
    }

    this->cpu.AddToReadyQueue(currentProcess); // add that process to the ready queue
    this->disks.getDisk(diskNumber).finishCurrentRequest(); // the disk stops helping that process and moves onto the next process
}

void SimOS::AccessMemoryAddress(unsigned long long address)
{
    if (this->cpu.getCurrentProcess().getPID() == 0) {
        throw std::logic_error("CPU IS IDLE.");
    }
    this->ram.accessMemory(this->cpu.getCurrentProcess().getPID(), address, this->pageSize);
}

// GetCPU returns the PID of the process currently using the CPU.
// If CPU is idle it returns NO_PROCESS (see the supplied definitions above).
int SimOS::GetCPU()
{
    return this->cpu.getCurrentProcess().getPID();
}

// GetReadyQueue returns the std::deque with PIDs of processes in the ready-queue
// where element in front corresponds start of the ready-queue.
std::deque<int> SimOS::GetReadyQueue()
{
    return this->cpu.convertReadyQueueToPIDs();
}

MemoryUsage SimOS::GetMemory() {
    MemoryUsage memory;
    for(int i = 0; i < this->ram.getMemory().size(); i++) {
        if(this->ram.getMemory()[i].PID !=  0) {
            memory.push_back(this->ram.getMemory()[i]);
        }
    }
    return memory;
}

FileReadRequest SimOS::GetDisk(int diskNumber)
{
    if(diskNumber >= numberOfDisks || diskNumber < 0) {
        throw std::out_of_range("DISK NUMBER ISN'T VALID.");
    }
    // If the disk is idle, GetDisk returns the default FileReadRequest object (with PID 0 and empty string in fileName)
    return this->disks.getDisk(diskNumber).getFileReadRequest();
}

std::deque<FileReadRequest> SimOS::GetDiskQueue(int diskNumber)
{
    if(diskNumber >= numberOfDisks || diskNumber < 0) {
        throw std::out_of_range("DISK NUMBER ISN'T VALID.");
    }
    return this->disks.getDisk(diskNumber).getDiskQueue();
}
