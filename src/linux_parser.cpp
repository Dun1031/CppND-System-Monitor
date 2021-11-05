#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h" 

using std::stof;
using std::string;
using std::vector;
using std::to_string;

// Read data from the filesystem
string LinuxParser::OperatingSystem() 
{
  string line;
  string key;
  string value;

  std::ifstream filestream(kOSPath);

  if (filestream.is_open()) 
  {
    while (std::getline(filestream, line)) 
    {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');

      std::istringstream linestream(line);

      while (linestream >> key >> value) 
      {
        if (key == filterPrettyName) 
        {
          std::replace(value.begin(), value.end(), '_', ' ');
          
          filestream.close();
          
          return value;
        }
      }
    }
  }

  return value;
}

// Read data from the filesystem
string LinuxParser::Kernel() 
{
  string os;
  string kernel;
  string version;
  string line;

  std::ifstream stream(kProcDirectory + kVersionFilename);

  if (stream.is_open()) 
  {
    std::getline(stream, line);
    std::istringstream linestream(line);

    linestream >> os >> version >> kernel;
    
    stream.close();
  }

  return kernel;
}

// Return a vector of process id's
vector<int> LinuxParser::Pids() 
{
  vector<int> pids;

  DIR* directory = opendir(kProcDirectory.c_str());

  struct dirent* file;

  while ((file = readdir(directory)) != nullptr)
  {
    // Is this a directory?
    if (file->d_type == DT_DIR)
    {
      string filename(file->d_name);

      if (std::all_of(filename.begin(), filename.end(), isdigit)) 
      {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }

  closedir(directory);

  return pids;
}

// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() 
{
  float memTot;
  float memFree;
  
  string line;
  string key;
  string value;

  std::ifstream filestream(kProcDirectory + kMeminfoFilename);

  if (filestream.is_open()) 
  { 
    while (std::getline(filestream, line)) 
    {
      std::istringstream linestream(line);

      while (linestream >> key >> value) 
      {
        if (key == filterMemTotal) 
        {
          memTot = stof(value); 
        }  
        if (key == filterMemFreeString) 
        {
          memFree = stof(value);
        }  
      }
    }
  	
    filestream.close();
  } 
  
  
  return  (memTot-memFree)/memTot;
}

// Read and return the system uptime
long LinuxParser::UpTime() 
{
  string ut;
  string line;
  
  std::ifstream stream(kProcDirectory + kUptimeFilename);

  if (stream.is_open()) 
  {
    std::getline(stream, line);

    std::istringstream linestream(line);

    linestream >> ut;
    
    stream.close();
  } 

  return std::stol(ut); 
}

// Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return UpTime()*sysconf(_SC_CLK_TCK); }

// Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) 
{  
  string line, value;

  std::vector<std::string> values;
  
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);

  if (filestream.is_open()) 
  {
    while (std::getline(filestream, line)) 
    {      
      std::istringstream linestream(line);

      while (linestream >> value) 
      {
        values.push_back(value);
        
        if(values.size() > 22)
        {
          filestream.close();
          
          return std::stol(values[13]) + std::stol(values[14]) + std::stol(values[15]) + std::stol(values[16]);
        }
      }
    }
  }

  return 0;
}

// Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() 
{
  string line;
  string key;
  string user;
  string nice;
  string system;
  string idle;
  string iowait;
  string irq;
  string softirq;
  string steal;
  string guest;
  string guestNice;

  std::ifstream filestream(kProcDirectory + kStatFilename);

  if (filestream.is_open()) 
  { 
    while (std::getline(filestream, line)) 
    {
      std::istringstream linestream(line);

      while (linestream >> key >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guestNice) 
      {
        if(key == filterCpu)
        {
          filestream.close();
          
          return std::stol(user) + std::stol(nice) + std::stol(system) + std::stol(idle) + 
                 std::stol(iowait) + std::stol(irq) + std::stol(softirq) + std::stol(steal) + 
                 std::stol(guest) + std::stol(guestNice);
        }
      }
    }
  }

  return 0; 
}

// Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() 
{ 
  string line;
  string key;
  string user;
  string nice;
  string system;
  string idle;
  string iowait;
  

  std::ifstream filestream(kProcDirectory + kStatFilename);

  if (filestream.is_open()) 
  { 
    while (std::getline(filestream, line)) 
    {
      std::istringstream linestream(line);

      while (linestream >> key >> user >> nice >> system >> idle >> iowait) 
      {
        if(key == filterCpu)
        {
          filestream.close();
          
          return std::stol(idle) + std::stol(iowait);
        }
      }
    }
  }

  return 0; 
}

// Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() 
{ 
  string line;
  string key;
  string user;
  string nice;
  string system;
  string idle;
  string iowait;
  string irq;
  string softirq;
  string steal;
  string guest;
  string guestNice;

  vector<string> v;

  std::ifstream filestream(kProcDirectory + kStatFilename);

  if (filestream.is_open()) 
  { 
    while (std::getline(filestream, line)) 
    {
      std::istringstream linestream(line);

      while (linestream >> key >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guestNice) 
      {
        if(key == filterCpu)
        {
          v = {user, nice, system, idle, iowait, irq, softirq, steal, guest, guestNice};

          filestream.close();
          
          return v;
        }
      }
    }
  }
 
  v = {"No Val"};

  return v; 
}

// Read and return the total number of processes
int LinuxParser::TotalProcesses() 
{ 
  string line;
  string key;
  string value;

  std::ifstream filestream(kProcDirectory + kStatFilename);

  if (filestream.is_open()) 
  {
    while (std::getline(filestream, line)) 
    {
      std::istringstream linestream(line);

      while (linestream >> key >> value) 
      {
        if (key == filterProcesses) 
        {
          filestream.close();
          
          return std::stoi(value);
        }
      }
    }
  }

  return std::stoi(value);
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() 
{   
  string line;
  string key;
  string value;

  std::ifstream filestream(kProcDirectory + kStatFilename);

  if (filestream.is_open()) 
  {
    while (std::getline(filestream, line)) 
    {
      std::istringstream linestream(line);

      while (linestream >> key >> value) 
      {
        if (key == filterProcsRunning) 
        {
          filestream.close();
          
          return std::stoi(value);
        }        
      }
    }
  }

  return std::stoi(value);
}

// Read and return the command associated with a process
string LinuxParser::Command(int pid) 
{
  string cmd;
  string line;
  
  std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);

  if (stream.is_open()) 
  {
    std::getline(stream, line);

    std::istringstream linestream(line);

    linestream >> cmd;
    
    stream.close();
  } 

  return cmd; 
}

// Read and return the memory used by a process
string LinuxParser::Ram(int pid) 
{ 
  string line;
  string key;
  string value;

  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  
  if (filestream.is_open()) 
  {
    while (std::getline(filestream, line)) 
    {
      std::istringstream linestream(line);
     
      while (linestream >> key >> value) 
      {
        if (key == filterProcMem)
        {
          filestream.close();
          
          return value;
        }
      }
    }
  }

  return value; 
}

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) 
{   
  string line;
  string key;
  string value;

  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);

  if (filestream.is_open()) 
  {
    while (std::getline(filestream, line)) 
    {
      std::istringstream linestream(line);

      while (linestream >> key >> value) 
      {
        if (key == filterUID) 
        {
          filestream.close();
          
          return value;
        }        
      }
    }
  }

  return value; 
}

// Read and return the user associated with a process
string LinuxParser::User(int pid) 
{ 
  string line;
  string key;
  string x;
  string value;

  std::ifstream filestream(kPasswordPath);

  if (filestream.is_open()) 
  {
    while (std::getline(filestream, line)) 
    {
      std::replace(line.begin(), line.end(), ':', ' ');
      
      std::istringstream linestream(line);

      while (linestream >> value >> x >> key) 
      {
        if (key == LinuxParser::Uid(pid)) 
        {
          filestream.close();
          
          return value;
        }        
      }
    }
  }

  return value; 
}

// Read and return the uptime of a process
long LinuxParser::UpTime(int pid) 
{   
  string line;
  string value;

  std::vector<std::string> values;
  
  std::ifstream filestream(kProcDirectory + to_string(pid) + "/"+ kStatFilename);

  if (filestream.is_open()) 
  {
    while (std::getline(filestream, line)) 
    {
      std::replace(line.begin(), line.end(), ':', ' ');
      
      std::istringstream linestream(line);

      while (linestream >> value) 
      {
        values.push_back(value);
        
        if(values.size() > 22)
        {
          auto r = values[21];

          values.clear();

          filestream.close();
          
          return std::stol(r);
        }
      }
    }
  }

  return 0;
}

// Read and return the processes cpu utilization
vector<string> LinuxParser::CpuUtilization(int pid) 
{   
  string line;
  string value;

  std::vector<std::string> values;
  
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);

  if (filestream.is_open()) 
  {
    while (std::getline(filestream, line)) 
    {      
      std::istringstream linestream(line);

      while (linestream >> value) 
      {
        values.push_back(value);
        
        if(values.size() > 22)
        {
          filestream.close();
          
          return values;
        }
      }
    }
  }

  return values;
}