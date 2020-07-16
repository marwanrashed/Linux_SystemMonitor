#include "processor.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
    long curr_idle, curr_total;
    float diff_total, diff_idle;
    curr_idle = LinuxParser::IdleJiffies();
    curr_total = LinuxParser::TotalTime();
    diff_total = curr_total - this->prev_total_;
    diff_idle = curr_idle - this->prev_idle_;

    float cpu_util = (diff_total - diff_idle) / diff_total;
    this->prev_idle_ = curr_idle;
    this->prev_total_ = curr_total;



    return cpu_util; }