#ifndef __OPSYS_H__
#define __OPSYS_H__

#include <iostream>
#include <vector>
#include <queue>
#include <unordered_set>
using namespace std;

#include "process.h"

class CompArrivalTime
{
public:
  bool operator() (Process* a, Process* b)
  {
    return a->arrival_time > b->arrival_time;
  }
};

class CompRearrivalTime
{
public:
  bool operator() (Process* a, Process* b)
  {
    return a->burst_completion_time > b->burst_completion_time;
  }
};

class OpSys
{
public:
  Process* running = NULL;
  std::queue<Process*> ready_fcfs;
  std::priority_queue<Process*, vector<Process*>, CompArrivalTime> ready_sjf; /* todo change comp */
  std::priority_queue<Process*, vector<Process*>, CompArrivalTime> ready_srt; /* todo change comp */
  std::priority_queue<Process*, vector<Process*>, CompArrivalTime> ready_rr; /* todo change comp */
  std::priority_queue<Process*, vector<Process*>, CompRearrivalTime> waiting;
  std::priority_queue<Process*, vector<Process*>, CompArrivalTime> unarrived;
  std::unordered_set<Process*> unfinished;
  int time = 0;
  int t_cs;

  void print_queue(const queue<Process*> &ready);
  void process_arrive(unsigned int current_time);
  void switch_out_cpu(unsigned int current_time);
  void complete_io(unsigned int current_time);
  void start_cpu_use(unsigned int current_time);
  void first_come_first_served();
  void shortest_job_first();
  void shortest_remaining_time();
  void round_robin();
};

#endif
