#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

//https://www.positioniseverything.net/cpp-undefined-reference/#:~:text=Key%20Takeaways-,What%20Does%20Undefined%20Reference%20Mean%20in%20C%2B%2B%3F,the%20executable%20program%20generation%20process.
/* CMakeFiles/monitor.dir/src/system.cpp.o: In function `System::Processes()':
system.cpp:(.text+0xa3): undefined reference to `Process::Process(int)'
collect2: error: ld returned 1 exit status
CMakeFiles/monitor.dir/build.make:175: recipe for target 'monitor' failed
make[2]: *** [monitor] Error 1
CMakeFiles/Makefile2:72: recipe for target 'CMakeFiles/monitor.dir/all' failed
make[1]: *** [CMakeFiles/monitor.dir/all] Error 2
Makefile:83: recipe for target 'all' failed
make: *** [all] Error 2
*/
//Getting Undefined Reference to error so creating a constructor for Process Class
Process::Process(int index){
    p = index;
    c = CpuUtilization();
}

// TODO: Return this process's ID
int Process::Pid(){
    return this->p;
}

// TODO: Return this process's CPU utilization
float Process::CpuUtilization(){
    float c_usage;
    float sys_up_time = LinuxParser::UpTime();
    float sys_strt_time = LinuxParser::UpTime(p);
    float sys_total_time = LinuxParser::ActiveJiffies(p);
    float temp = sys_strt_time / sysconf(_SC_CLK_TCK);
    float sys_seconds = sys_up_time - temp;
    if(sys_seconds > 0.0){
        float temp2 = ((float)sysconf(_SC_CLK_TCK)) / sys_seconds;
        c_usage = sys_total_time / temp2;
    }
    return c_usage;
}

// TODO: Return the command that generated this process
string Process::Command(){
    return LinuxParser::Command(p);
}

// TODO: Return this process's memory utilization
string Process::Ram(){
    string rm = LinuxParser::Ram(p);
    std::stringstream r_stream(rm);
    r_stream >> r;
    return rm;
}

// TODO: Return the user (name) that generated this process
string Process::User(){
    return LinuxParser::User(p);
}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime(){
    return LinuxParser::UpTime(p);
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a)const{
    if(a.c > c){
        return true;
    }
    return false;
    if(this->r < a.r){
        return true;
    }
    return false;
}
