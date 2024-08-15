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
    return a->burstCompletionTime() > b->burstCompletionTime();
  }
};

class OpSys
{
public:
  Process* running = NULL;
  std::priority_queue<Process*, vector<Process*>, CompArrivalTime> ready_fcfs;
  std::priority_queue<Process*, vector<Process*>, CompArrivalTime> ready_sjf; /* todo change comp */
  std::priority_queue<Process*, vector<Process*>, CompArrivalTime> ready_srt; /* todo change comp */
  std::priority_queue<Process*, vector<Process*>, CompArrivalTime> ready_rr; /* todo change comp */
  std::priority_queue<Process*, vector<Process*>, CompRearrivalTime> waiting;
  std::priority_queue<Process*, vector<Process*>, CompArrivalTime> unarrived;
  int time = 0;

  void first_come_first_served();
  void shortest_job_first();
  void shortest_remaining_time();
  void round_robin();
};

#endif
