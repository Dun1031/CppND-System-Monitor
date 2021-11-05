#include "processor.h"
#include "linux_parser.h"

// Return the processors CPU utilization
float Processor::Utilization() 
{ 
    Processor::Utilization(LinuxParser::CpuUtilization());
    
    return Processor::utilization_;  
}

// Calculate the processors CPU utilization
void Processor::Utilization(std::vector<std::string> cp)
{
    float guest = std::stof(cp[8]);
    float userTime = std::stof(cp[0]) - guest;
    float guestNice = std::stof(cp[9]);
    float niceTime = std::stof(cp[1]) - guestNice; 

    float idleAllTime = std::stof(cp[3]) + std::stof(cp[4]);
    float systemAllTime = std::stof(cp[2]) + std::stof(cp[5]) + std::stof(cp[6]);
    float virtualAllTime = guest + guestNice;
    float totalTime = userTime + niceTime + systemAllTime + idleAllTime + std::stof(cp[7]) + virtualAllTime;

    
    Processor::utilization_ = 1-(idleAllTime/totalTime);
}
