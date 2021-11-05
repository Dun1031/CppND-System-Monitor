#ifndef PROCESS_H
#define PROCESS_H
  
#include <string>
#include <vector>
#include <unistd.h>

#include "format.h"


/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process 
{
 public:
  // Define the constructor for this class
  Process(int p, std::string u, std::string c, std::string r, long int ut, std::vector<std::string> v, long sysUT) : pId_(p), user_(u), command_(c) 
  {
    // Calculate the processes total ram
    ram_ = std::to_string(std::stoi(r)/1000);

    // Calculate the processes uptime
    upTime_ = sysUT - ut/sysconf(_SC_CLK_TCK);

    // Calculate the processes total time
    float tt = std::stof(v[15]) + std::stof(v[16]) + std::stof(v[17]) + std::stof(v[18]);

    // Calculate seconds
    float s = tt/sysconf(_SC_CLK_TCK);

    // Calculate the cpu utilization
    cpuUtilization_ = s/upTime_;
  }

  int Pid();

  std::string Ram();
  std::string User();
  std::string Command();

  long int UpTime();

  float CpuUtilization();

  bool operator<(Process const& a) const; 

  private:
  int strLen_ = 40;

  int pId_;

  long int upTime_;
  
  float cpuUtilization_;

  bool _operator;

  std::string ram_;
  std::string user_;
  std::string command_;
};

#endif