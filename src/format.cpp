#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long int seconds) { 
// auto days = seconds/60/60/24;
int hours = ((seconds/60)/60)%24;
int minutes = (seconds/60)%60;
int seconds_ = seconds%60;

string hours_string, minutes_string, seconds_string;
if (hours < 10 ) 
    hours_string  = '0' + std::to_string(hours);
else
{
    hours_string = std::to_string(hours);
}

if (minutes < 10 ) 
    minutes_string  = '0' + std::to_string(minutes); 
else
{
    minutes_string = std::to_string(minutes);
}
if (seconds_ < 10 ) 
    seconds_string  = '0' + std::to_string(seconds_);
else
{
    seconds_string  = std::to_string(seconds_);

}

string Elapsed_ = hours_string + ":" + minutes_string + ":" +  seconds_string; 
    return Elapsed_; }