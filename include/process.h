#ifndef PROCESS_H
#define PROCESS_H

#include <linux_parser.h>
#include <string>
using namespace std;

/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  void Pid (const int pid);
  int Pid ()const;                               // TODO: See src/process.cpp
  std::string User();                      // TODO: See src/process.cpp
  std::string Command();                   // TODO: See src/process.cpp
  float CpuUtilization()const;                  // TODO: See src/process.cpp
  std::string Ram();                       // TODO: See src/process.cpp
  long int UpTime();                       // TODO: See src/process.cpp
  bool operator<(Process const& a) const;  // TODO: See src/process.cpp

  // TODO: Declare any necessary private members
 private:
 int pid_;
 float Cpu_util_;
};

#endif