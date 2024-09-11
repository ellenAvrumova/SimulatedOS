// Ellen Avrumova

#include "MemoryItem.h"
#include <algorithm>

class MemoryManager {
    private:
        MemoryUsage memory;
        std::vector<int> frameCounters;
        int counter;
    public:
        MemoryManager();
        MemoryManager(unsigned long long frames);
        MemoryUsage getMemory();
        void accessMemory(int pid, unsigned long long addr, unsigned int pgSize);
        void removeFromMemory(int pid);
};