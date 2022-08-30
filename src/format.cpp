#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds){
    long hrs;
    long mins;
    long secs;
    string res;
    hrs = seconds/3600;
    mins = seconds/60;
    mins = int(mins%60);
    secs = int(seconds%60);
    res = std::to_string(hrs)+":"+std::to_string(mins)+":"+std::to_string(secs);
    return res;
}
