#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include<iostream>
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
      // while (linestream >> key >> value) {
        linestream >> key >> value;
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        // }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version ,kernel;
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
float LinuxParser::MemoryUtilization() { 
  string mem_t_a , mem_f_a;
  string memtotal_key, memtotal_value;
  string memfree_key, memfree_value;
  string line;
  std::ifstream mem_util_file(kProcDirectory+kMeminfoFilename);
  while (mem_util_file.is_open() && std::getline(mem_util_file,line)){
    // std::replace (line.begin(), line.end(),":" , " ");
    std::istringstream mem_total_line (line);
    std::istringstream mem_free_line (line);
    mem_total_line >> memtotal_key >> mem_t_a;
    if (memtotal_key == "MemTotal:") {
      memtotal_value = mem_t_a;
    }
    mem_free_line >> memfree_key >> mem_f_a;
    if (memfree_key == "MemFree:") {
      memfree_value = mem_f_a;
    }
  }
  
      return   float ((stof(memtotal_value) - stof(memfree_value))/stof(memtotal_value));
 }

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  string uptime_, idle_time;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime_ >> idle_time;
  }

  return std::stol(uptime_); }

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {

   return ActiveJiffies () + IdleJiffies (); }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  std::vector<long> cpu_v = LinuxParser::CpuUtilization();

  long active_jiffies = cpu_v[LinuxParser::CPUStates::kUser_] + cpu_v[LinuxParser::CPUStates::kNice_]
                      + cpu_v[LinuxParser::CPUStates::kSystem_] + cpu_v[LinuxParser::CPUStates::kIRQ_]
                      + cpu_v[LinuxParser::CPUStates::kSoftIRQ_] + cpu_v[LinuxParser::CPUStates::kSteal_] ; 

  return active_jiffies; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  std::vector<long> cpu_v = LinuxParser::CpuUtilization();

  long idle_jiffies = cpu_v[LinuxParser::CPUStates::kIdle_] + cpu_v[LinuxParser::CPUStates::kIOwait_]; 
  
  return idle_jiffies; }

long LinuxParser::VirtalJiffies() { 
  std::vector<long> cpu_v = LinuxParser::CpuUtilization();

  long virtaljiffies = cpu_v[LinuxParser::CPUStates::kGuest_] + cpu_v[LinuxParser::CPUStates::kGuestNice_]; 
  return virtaljiffies; }




// TODO: Read and return CPU utilization
vector<long> LinuxParser::CpuUtilization() { 
  string line, cpu_key, cpu_value;
  string cpu_path {kProcDirectory + kStatFilename }; 
  std::ifstream cpu_file (cpu_path);
  std::vector<long> cpu_values{};
  if (cpu_file.is_open()){
    std::getline(cpu_file, line);
    std::istringstream cpu_aggr_stream (line);
    cpu_aggr_stream >> cpu_key;
    if (cpu_key == "cpu"){
      while (cpu_aggr_stream >> cpu_value){
        // std::cout << "Cpu_value is : " << cpu_value << std::endl;
        cpu_values.emplace_back(stol(cpu_value));
      }
    }
  }
  return cpu_values; }

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string process_key,process_value, line;
  std::ifstream process_filestream (kProcDirectory+kStatFilename);
  while (process_filestream.is_open() && std::getline(process_filestream,line)) {
    std::istringstream process_line (line);
    process_line >> process_key >> process_value; 
    if (process_key == "processess")
      break;
  }
  return std::stoi(process_value); }

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {   
  string running_process_key,running_process_value, line;
  std::ifstream process_filestream (kProcDirectory+kStatFilename);
  while (process_filestream.is_open() && std::getline(process_filestream,line)) {
    std::istringstream process_line (line);
    process_line >> running_process_key >> running_process_value; 
    if (running_process_key == "processess")
      break;
  }
  return std::stoi(running_process_value); }

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid[[maybe_unused]]) { return 0; }