#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
 
using std::string;
using std::vector;
using std::to_string;

// Return this process's ID
int Process::Pid() { return Process::pId_; }

// Return this process's CPU utilization
float Process::CpuUtilization() { return Process::cpuUtilization_; }

// Return the command that generated this process
string Process::Command() { return Format::TruncateString(Process::command_, strLen_); }

// Return this process's memory utilization
string Process::Ram() { return Process::ram_; }

// Return the user that launched this process
string Process::User() { return Process::user_; }

// Return the uptime for this proccess
long int Process::UpTime() { return Process::upTime_; }

// Overload < to sort by CPU Utilization
bool Process::operator<(Process const& a) const 
{
    return a.cpuUtilization_ < this->cpuUtilization_;    
}