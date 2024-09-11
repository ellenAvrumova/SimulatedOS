// Ellen Avrumova

#include <iostream>
#include "DiskManager.h"

DiskManager::DiskManager() { }

// Create a vector of disks the size of the number of disks specified by the SimOS constructor
DiskManager::DiskManager(int numOfDisks)
{
    for(int i = 0; i < numOfDisks; i++) {
        this->diskNums.push_back(Disk(i));
    }
}

std::vector<Disk>& DiskManager::getDiskList() {
    return this->diskNums;
}

// returns a reference to the Disk so the object returned can be directly modified.
Disk& DiskManager::getDisk(int diskNumber)
{
    return this->diskNums[diskNumber];
}
