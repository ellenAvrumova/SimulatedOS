// Ellen Avrumova

#include <iostream>
#include "MemoryManager.h"

MemoryManager::MemoryManager() { }

MemoryManager::MemoryManager(unsigned long long frames) {
    this->memory.resize(frames);
    this->frameCounters.resize(frames);
    std::fill(frameCounters.begin(), frameCounters.end(), 0); // fill frameCounters with 0s
    this->counter = 1;
}

MemoryUsage MemoryManager::getMemory() {
    return this->memory;
}

void MemoryManager::accessMemory(int pid, unsigned long long addr, unsigned int pgSize) {
    unsigned long long pageNum = addr/pgSize;
    bool exists = false;
    for(unsigned long long i = 0; i < this->memory.size(); i++) { // loop through each frame in memory
        if((this->memory[i].PID == pid) && (this->memory[i].pageNumber == pageNum)){ // if the process and pageNumber already exists
            exists = true;
            frameCounters[i] = counter;
            counter++;
            return;
        }
    }
    if(!exists) { // if there isn't a frame w/ the same process and pageNum, and add it to an empty frame
        for(unsigned long long i = 0; i < this->memory.size(); i++) { // loop through each frame in memory
            if(this->memory[i].PID == 0) { // if there's an empty frame, fill it with a memory item
                this->memory[i] = MemoryItem{pageNum, i, pid};
                frameCounters[i] = counter;
                counter++;
                return;
            }
        }
    }
    // if memory is full, replace the least recently used item with the new one
    std::vector<int>::iterator min = std::min_element(frameCounters.begin(), frameCounters.end());
    unsigned long long index = std::distance(frameCounters.begin(), min);
    this->memory[index] = MemoryItem{pageNum, index, pid};
    this->frameCounters[index] = counter;
    counter++;
}

void MemoryManager::removeFromMemory(int pid) {
    for(unsigned long long i = 0; i < this->memory.size(); i++) {
        if(this->memory[i].PID == pid) {
            this->memory[i].PID = 0;
            this->frameCounters[i] = 0;
        }
    }
}