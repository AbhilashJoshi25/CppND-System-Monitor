#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization(){
  //Used Info from https://stackoverflow.com/questions/41224738/how-to-calculate-system-memory-usage-from-proc-meminfo-like-htop
  //Total used memory = MemTotal - MemFree
  //Non cache/buffer memory (green) = Total used memory - (Buffers + Cached memory)
  //Buffers (blue) = Buffers
  //Cached memory (yellow) = Cached + SReclaimable - Shmem
  //Swap = SwapTotal - SwapFree
  float MemTotal, MemFree, res_memory;
  //int i = 0;
  string k, v, l;
  //vector<int>per_unit_memory = {};
  std::ifstream curr_stream(kProcDirectory + kMeminfoFilename);
  if(curr_stream.is_open()){
    while(getline(curr_stream, l)){
      std::replace(l.begin(),l.end(),':',' ');
      std::istringstream lstream(l);
      while(lstream >> k >> v){
        if(k == "MemTotal"){
          MemTotal = std::stoi(v);
          //per_unit_memory.push_back(MemTotal);
          //i++;
        }
        if(k == "MemFree"){
          MemFree = std::stoi(v);
          //per_unit_memory.push_back(MemFree);
          //i++;
        }
      }
    }
    //res_memory = (per_unit_memory[0] - per_unit_memory[1])/(float(per_unit_memory[0]));
    //return res_memory;
  }
  res_memory = MemTotal - MemFree;
  return res_memory / MemTotal;
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime(){
  //run time
  // convert string to integer
  long t3 = 0;
  string t1, t2;
  string l;
  std::ifstream stm(kProcDirectory + kUptimeFilename);
  if(stm.is_open()){
    while(std::getline(stm, l)){
      std::istringstream lstream(l);
      while(lstream >> t1 >> t2){
        t3 = std::stol(t1);
      }
      return t3;
    }
    //res = total_time_in - i_time;
  }
  //return std::stoi(total_time_in);
  return t3;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies(){
  float res = 0.0;
  int i = 0;
  vector<string>v= {};
  v = LinuxParser::CpuUtilization();
  while(i < v.size()){
    res += std::stoi(v[i]);
    i++;
  }
  return res;
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid){
  string v, l;
  string p = to_string(pid);
  long int res;
  vector<string>ajf;
  std::ifstream stream(kProcDirectory +  p + kStatFilename);
  if(stream.is_open()){
    std::getline(stream, l);
    std::istringstream lstream(l);
    while(lstream >> v){
      ajf.push_back(v);
    }
  }
  for(int i = 13; i <= 16; i++){
    res += std::stoi(ajf[i]);
  }
  return res;
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies(){
  vector<string>v = {};
  v = LinuxParser::CpuUtilization();
  long int sys_aj;
  sys_aj = stoi(v[CPUStates::kUser_]) + stoi(v[CPUStates::kNice_]) +
   stoi(v[CPUStates::kSteal_]) + stoi(v[CPUStates::kSoftIRQ_]) + 
   stoi(v[CPUStates::kIRQ_]) + stoi(v[CPUStates::kSystem_]);
  return sys_aj;
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies(){
  vector<string>v = {};
  v = LinuxParser::CpuUtilization();
  long int sys_ij;
  sys_ij = stoi(v[CPUStates::kIdle_]) + stoi(v[CPUStates::kIOwait_]);
  return sys_ij;
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization(){
  string l, v;
  int i = 0;
  vector<string>c = {};
  std::ifstream cpu_stream(kProcDirectory + kStatFilename);
  if(cpu_stream.is_open()){
    std::getline(cpu_stream, l);
    std::istringstream lstream(l);
    lstream >> v;
    while(i < 10){
      lstream >> v;
      c.push_back(v);
      i++;
    }
  }
  return c;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses(){
  string k, v, l;
  long int tp;
  std::ifstream tp_stream(kProcDirectory + kStatFilename);
  if(tp_stream.is_open()){
    while(getline(tp_stream, l)){
      std::istringstream lstream(l);
      while(lstream >> k >> v){
        if(k == "processess"){
          tp = stoi(v);
          return tp;
        }
      }
    }
  }
  return tp;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses(){
  string k, v, l;
  long int rp;
  std::ifstream rp_stream(kProcDirectory + kStatFilename);
  if(rp_stream.is_open()){
    while(getline(rp_stream, l)){
      std::istringstream lstream(l);
      while(lstream >> k >> v){
        if(k == "procs_running"){
          rp = stoi(v);
          return rp;
        }
      }
    }
  }
  return rp;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid){
  string k, v, l;
  string p = to_string(pid);
  std::ifstream c_stream(kProcDirectory + p + kStatFilename);
  if(c_stream.is_open()){
    std::getline(c_stream, l);
    std::istringstream lstream(l);
    lstream >> k >>v;
    return v;
  }
  return v;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid){
  string k, v, l;
  long int r;
  string p = to_string(pid);
  std::ifstream r_stream(kProcDirectory + p + kStatusFilename);
  if(r_stream.is_open()){
    while(getline(r_stream, l)){
      std::istringstream lstream(l);
      while(lstream >> k >> v){
        if(k == "VmSize"){
          r = stoi(v)/1024;
          return to_string(r);
        }
      }
    }
  }
  return to_string(r);
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid){
  string k, v, l;
  string p = to_string(pid);
  std::ifstream u_stream(kProcDirectory + p + kStatusFilename);
  if(u_stream.is_open()){
    while(getline(u_stream, l)){
      std::istringstream lstream(l);
      while(lstream >> k >> v){
        if(k == "Uid"){
          return v;
        }
      }
    }
  }
  return v;
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid){
  string k, v, p, l, userid;
  userid = LinuxParser::Uid(pid);
  std::ifstream us_stream(kPasswordPath);
  if(us_stream.is_open()){
    while(getline(us_stream, l)){
      std::replace(l.begin(),l.end(),':',' ');
      std::istringstream lstream(l);
      while(lstream >> k >> v >> p){
        if(p == userid){
          return k;
        }
      }
    }
  }
  return k;
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid){
  string k, v, l, p;
  p = to_string(pid);
  long int utime;
  int i = 0;
  vector<string>j = {};
  std::ifstream up_stream(kProcDirectory + p + kStatFilename);
  if(up_stream.is_open()){
    std::getline(up_stream, l);
    std::istringstream lstream(l);
    while(i < 22){
      lstream >> v;
      j.push_back(v);
      i++;
    }
    utime = std::stoi(j[21]) / sysconf(_SC_CLK_TCK);
  }
  return utime;
}

