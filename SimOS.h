// Ellen Avrumova

#include <string>
#include <deque>
#include <vector>
#include <unordered_map>
#include "CPUManager.h"
#include "DiskManager.h"
#include "MemoryManager.h"
#include <iostream>

constexpr int NO_PROCESS{ 0 };
class SimOS
{
private:
    int numberOfDisks;
    unsigned long long amountOfRAM;
    unsigned int pageSize;
    CPUManager cpu;
    DiskManager disks;
    MemoryManager ram;
    int previousPID;
    std::vector<ProcessManager> waitList;
    std::vector<ProcessManager> zombieList;
    std::unordered_map<int, ProcessManager> processMap;
public:
    SimOS(int numberOfDisks, unsigned long long amountOfRAM, unsigned int pageSize);
    void NewProcess();
    void SimFork();
    void SimExit();
    void SimWait();
    void TimerInterrupt();
    void DiskReadRequest(int diskNumber, std::string fileName);
    void DiskJobCompleted(int diskNumber);
    void AccessMemoryAddress(unsigned long long address);
    int GetCPU();
    std::deque<int> GetReadyQueue();
    MemoryUsage GetMemory();
    FileReadRequest GetDisk(int diskNumber);
    std::deque<FileReadRequest> GetDiskQueue(int diskNumber);
    void destroyChildren(ProcessManager& pcb);
};