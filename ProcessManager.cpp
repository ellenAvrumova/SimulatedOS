// Ellen Avrumova

#include <iostream>
#include "ProcessManager.h"

ProcessManager::ProcessManager() {
    this->PID = 0;
}

ProcessManager::ProcessManager(int ID)
{
    this->PID = ID; // assigns a unique PID to each process
}

int ProcessManager::getPID() {
    return this->PID;
}

void ProcessManager::resetProcess() {
    this->PID = 0;
}

void ProcessManager::setParent(int ID) {
    this->parentPID = ID;
}

void ProcessManager::addChild(int ID) {
    this->childrenPIDs.push_back(ID);
}

std::vector<int>& ProcessManager::getChildrenList() {
    return this->childrenPIDs;
}

void ProcessManager::removeChild(int childIndex) {
    auto zombie = this->childrenPIDs.begin() + childIndex;
    this->childrenPIDs.erase(zombie);
}

int ProcessManager::getParentPID() {
    return this->parentPID;
}