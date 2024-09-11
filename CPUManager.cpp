// Ellen Avrumova

#include "CPUManager.h"
#include <iostream>

CPUManager::CPUManager()
{
}

void CPUManager::AddToReadyQueue(ProcessManager& newProcess)
{
    if(this->CPU.getPID() == 0) { // if no processes have been created yet, the CPU begins execution of this process
        this->CPU = newProcess;
    }
    else {
        this->ReadyQueue.push_back(newProcess); // if a process is already being executed, the process is added to the Ready Queue
    }
}

std::deque<ProcessManager> CPUManager::getReadyQueue()
{
    return this->ReadyQueue;
}

ProcessManager& CPUManager::getCurrentProcess()
{
    return this->CPU;
}

void CPUManager::timerInterrupt()
{
    // Add the current executing process to the end of the Ready Queue and start execution of the next process in the Ready Queue
    if(!this->ReadyQueue.empty()) {
        ProcessManager currentProcess = this->CPU;
        ProcessManager nextProcess = this->ReadyQueue.front();
        this->CPU = nextProcess;
        this->ReadyQueue.pop_front();
        this->ReadyQueue.push_back(currentProcess);
    }
}

// After a process requests to read a file from a disk
// the CPU stops executing that process and
// starts execution of the next process without adding this current process to the Ready Queue
void CPUManager::stopUsingCPU()
{
    if(this->ReadyQueue.empty()) {
        this->CPU.resetProcess();
    }
    else {
        ProcessManager currentProcess = this->CPU;
        ProcessManager nextProcess = this->ReadyQueue.front();
        this->CPU = nextProcess;
        this->ReadyQueue.pop_front();
    }
}

std::deque<int> CPUManager::convertReadyQueueToPIDs() {
    std::deque<int> readyQueuePIDs;
    for(int i = 0; i < this->ReadyQueue.size(); i++) {
        readyQueuePIDs.push_back(ReadyQueue[i].getPID());
    }
    return readyQueuePIDs;
}

bool CPUManager::removeFromReadyQueue(int processPID) {
    for(int i = 0; i < this->ReadyQueue.size(); i++) {
        if(this->ReadyQueue[i].getPID() == processPID) {
            this->ReadyQueue.erase(ReadyQueue.begin() + i);
            return true;
        }
    }
    return false;
}