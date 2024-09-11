// Ellen Avrumova

#include <deque>
#include <string>
#include <vector>
#include "Disk.h"

class DiskManager
{
    private:
        std::vector<Disk> diskNums;
    public:
        DiskManager();
        DiskManager(int numOfDisks);
        std::vector<Disk>& getDiskList();
        Disk& getDisk(int diskNum);
};