// Ellen Avrumova

#include "Disk.h"
#include <iostream>

Disk::Disk(int diskNumber) {
    this->DiskID = diskNumber;
}

FileReadRequest Disk::getFileReadRequest() {
    return this->currentReadRequest;
}

int Disk::getDiskID() { //remove for submission
    return this->DiskID;
}

void Disk::readRequest(ProcessManager newProcess, std::string file)
{
    // if there isn't a current ongoing read request,
    // the request that was just made becomes the current read request
    int currentProcessPID = newProcess.getPID();
    FileReadRequest newReadRequest{currentProcessPID, file};
    if(this->currentReadRequest.PID == 0) {
        this->currentReadRequest = newReadRequest;
        this->currentProcess = newProcess;
    }
    // if the disk is dealing with a request, add the request to the I/O queue (Disk Queue)
    else {
        this->IOqueue.push_back(newProcess);
        this->DiskQueue.push_back(newReadRequest);
    }
}

std::deque<FileReadRequest> Disk::getDiskQueue()
{
    return this->DiskQueue;
}

void Disk::finishCurrentRequest()
{
    if(this->DiskQueue.empty()) {
        this->currentReadRequest.PID = 0;
        this->currentReadRequest.fileName = "";
        this->currentProcess.resetProcess();
    }
    else {
        FileReadRequest nextRequest = this->DiskQueue.front();
        ProcessManager nextProcess = this->IOqueue.front();
        this->currentReadRequest = nextRequest;
        this->currentProcess = nextProcess;
        this->DiskQueue.pop_front();
        this->IOqueue.pop_front();
    }
}

ProcessManager& Disk::getCurrentProcess() {
    return this->currentProcess;
}

void Disk::removeFromIOQueue(int processPID) {
    if(processPID == currentProcess.getPID()) {
        finishCurrentRequest();
    }
    else {
        for(int i = 0; i < this->IOqueue.size(); i++) {
            if(this->IOqueue[i].getPID() == processPID) {
                this->IOqueue.erase(IOqueue.begin() + i);
                this->DiskQueue.erase(DiskQueue.begin() + i);
            }
        }
    }
}