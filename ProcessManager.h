// Ellen Avrumova

#pragma once
#include <vector>

class ProcessManager {
    private:
        int PID;
        int parentPID;
        std::vector<int> childrenPIDs;
    public:
        ProcessManager();
        ProcessManager(int ID);
        int getPID(); 
        void resetProcess();
        void setParent(int ID);
        void addChild(int ID);
        std::vector<int>& getChildrenList();
        void removeChild(int childIndex);
        int getParentPID();
};