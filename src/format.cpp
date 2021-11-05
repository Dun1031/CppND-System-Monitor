#include <string>

#include "format.h" 

using std::string;

// Helper Function to format seconds into HH:MM:SS
string Format::ElapsedTime(long seconds) 
{ 
  int h, m;
  
  h = seconds/3600;
  seconds = seconds%3600;
  
  m = seconds/60;
  seconds = seconds%60;
    
  return ((h < 10) ? "0" : "") + std::to_string(h) + ":" + ((m < 10) ? "0" : "")  + std::to_string(m) + ":" + 
         ((seconds < 10) ? "0" : "") + std::to_string(seconds); 
}

// Helper function to limit strings lengths. Default 50.
std::string Format::TruncateString(std::string str, int strLen = 50)
{
    return str.substr(0, strLen) + "...";    
}