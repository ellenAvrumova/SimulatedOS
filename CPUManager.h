// Ellen Avrumova

#include <deque>
#include "ProcessManager.h"

class CPUManager
{
    private:
        std::deque<ProcessManager> ReadyQueue;
        ProcessManager CPU;
    public:
        CPUManager();
        void AddToReadyQueue(ProcessManager& newProcess); 
        std::deque<ProcessManager> getReadyQueue(); 
        ProcessManager& getCurrentProcess();
        void timerInterrupt();
        void stopUsingCPU();
        std::deque<int> convertReadyQueueToPIDs();
        bool removeFromReadyQueue(int processPID);
};