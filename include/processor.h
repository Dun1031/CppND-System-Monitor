#ifndef PROCESSOR_H
#define PROCESSOR_H
 
#include <string>
#include <vector>


class Processor 
{
 public:  
  void Utilization(std::vector<std::string> cp);
  float Utilization();

 private:
  float utilization_;
};

#endif