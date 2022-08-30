#include "processor.h"
#include "linux_parser.h"
#include <iostream>

// TODO: Return the aggregate CPU utilization
float Processor::Utilization(){
    float iln = LinuxParser::IdleJiffies();
    float n_iln = LinuxParser::ActiveJiffies();
    //float tln = LinuxParser::Jiffies();
    float temp = iln + n_iln;
    float tl = temp - tlp;
    float idl = iln - ilp;

    //store values
    tlp = temp;
    ilp = iln;

    //return percentage utilization
    return (tl - idl)/tl;
}
