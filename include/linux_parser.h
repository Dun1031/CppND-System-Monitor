#ifndef SYSTEM_PARSER_H
#define SYSTEM_PARSER_H
 
#include <fstream>
#include <regex>
#include <string>

namespace LinuxParser {
// Paths
const std::string kProcDirectory{"/proc/"};
const std::string kCmdlineFilename{"/cmdline"};
const std::string kCpuinfoFilename{"/cpuinfo"};
const std::string kStatusFilename{"/status"};
const std::string kStatFilename{"/stat"};
const std::string kUptimeFilename{"/uptime"};
const std::string kMeminfoFilename{"meminfo"};
const std::string kVersionFilename{"/version"};
const std::string kOSPath{"/etc/os-release"};
const std::string kPasswordPath{"/etc/passwd"};

const std::string filterPrettyName("PRETTY_NAME");
const std::string filterProcesses("processes");
const std::string filterProcsRunning("procs_running");
const std::string filterMemTotal("MemTotal:");
const std::string filterMemFreeString("MemFree:");
const std::string filterCpu("cpu");
const std::string filterUID("Uid:");
const std::string filterProcMem("VmSize:");


// System
float MemoryUtilization();

long UpTime();

int TotalProcesses();
int RunningProcesses();

std::string Kernel();
std::vector<int> Pids();
std::string OperatingSystem();

// CPU
enum CPUStates {
  kUser_ = 0,
  kNice_,
  kSystem_,
  kIdle_,
  kIOwait_,
  kIRQ_,
  kSoftIRQ_,
  kSteal_,
  kGuest_,
  kGuestNice_
};

std::vector<std::string> CpuUtilization();
std::vector<std::string> CpuUtilization(int pid);

long Jiffies();
long IdleJiffies();
long ActiveJiffies();
long ActiveJiffies(int pid);

// Processes
std::string Ram(int pid);
std::string Uid(int pid);
std::string User(int pid);
std::string Command(int pid);

long int UpTime(int pid);
};// namespace LinuxParser

#endif