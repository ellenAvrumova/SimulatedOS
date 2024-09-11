// Ellen Avrumova

#include <deque>
#include <string>
#include <vector>
#include "FileReadRequest.h"
#include "ProcessManager.h"

class Disk
{
    private:
        int DiskID;
        std::deque<ProcessManager> IOqueue;
        std::deque<FileReadRequest> DiskQueue;
        FileReadRequest currentReadRequest;
        ProcessManager currentProcess;
    public:
        Disk(int diskNumber);
        FileReadRequest getFileReadRequest();
        int getDiskID(); 
        void readRequest(ProcessManager newProcess, std::string file);
        std::deque<FileReadRequest> getDiskQueue();
        void finishCurrentRequest();
        ProcessManager& getCurrentProcess();
        void removeFromIOQueue(int processPID);
};