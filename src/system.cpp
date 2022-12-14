#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;
/*You need to complete the mentioned TODOs in order to satisfy the rubric criteria "The student will be able to extract and display basic data about the system."

You need to properly format the uptime. Refer to the comments mentioned in format. cpp for formatting the uptime.*/

// TODO: Return the system's CPU
Processor& System::Cpu(){
    return cpu_;
}

/*
// TODO: Return a container composed of the system's processes
//This fuction is causing segmentation fault
vector<Process>& System::Processes(){
    vector<int>line_of_pids = LinuxParser::Pids();
    processes_ = {};
    int i = 0;
    while(i < line_of_pids.size()){
        processes_.emplace_back(i);
        i++;
    }
    std::sort(processes_.begin(), processes_.end());
    return processes_;
}
*/
// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes(){
    processes_.clear();
    vector<int>line_of_pids = {};
    line_of_pids = LinuxParser::Pids();
    for (auto i : line_of_pids){
        Process ps(i);
        processes_.push_back(ps);
    }
    std::sort(processes_.begin(), processes_.end());
    return processes_;
}

// TODO: Return the system's kernel identifier (string)
std::string System::Kernel(){
    return LinuxParser::Kernel();
}

// TODO: Return the system's memory utilization
float System::MemoryUtilization(){
    return LinuxParser::MemoryUtilization();
}

// TODO: Return the operating system name
std::string System::OperatingSystem(){
    return LinuxParser::OperatingSystem();
}

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses(){
    return LinuxParser::RunningProcesses();
}

// TODO: Return the total number of processes on the system
int System::TotalProcesses(){
    return LinuxParser::TotalProcesses();
}

// TODO: Return the number of seconds since the system started running
long int System::UpTime(){
    return LinuxParser::UpTime();
}
