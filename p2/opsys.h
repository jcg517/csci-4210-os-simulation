#ifndef __OPSYS_H__
#define __OPSYS_H__

#include <iostream>
#include <vector>
#include <queue>
#include <unordered_set>
#include <cstring>
#include "process.h"

#define TRUNCATE true
#define TRUNC_TIME 10000

class CompArrivalTime
{
public:
  bool operator() (Process* a, Process* b)
  {
    return a->arrival_time > b->arrival_time;
  }
};

class CompPredBurstTime
{
public:
  bool operator() (Process* a, Process* b)
  {
    if (a->getTau() == b->getTau())
    {
      return std::strcmp(a->id, b->id) > 0;
    }
    return a->getTau() > b->getTau();
  }
};

class CompBurstCompletionTime
{
public:
  bool operator() (Process* a, Process* b)
  {
    return a->burstCompletionTime() > b->burstCompletionTime();
  }
};

void print_queue(const std::queue<Process*> &ready);
void print_priority_queue(const std::priority_queue<Process*, std::vector<Process*>, CompPredBurstTime> &ready);

class OpSys
{
public:
  Process* running = NULL;
  Process* switching_to_run = NULL;
  std::queue<Process*> ready_fcfs;
  std::priority_queue<Process*, std::vector<Process*>, CompPredBurstTime> ready_sjf;
  std::priority_queue<Process*, std::vector<Process*>, CompPredBurstTime> ready_srt; /* todo change comp */
  std::queue<Process*> ready_rr; 
  std::priority_queue<Process*, std::vector<Process*>, CompBurstCompletionTime> waiting;
  std::priority_queue<Process*, std::vector<Process*>, CompArrivalTime> unarrived;
  std::unordered_set<Process*> unfinished;
  int time = 0;
  int t_cs;
  int tslice;
  
  /* FCFS */
  void process_arrive_fcfs(unsigned int current_time);
  void switch_out_cpu_fcfs(unsigned int current_time);
  void complete_io_fcfs(unsigned int current_time);
  void start_cpu_use_fcfs(unsigned int current_time);
  void first_come_first_served();
  
  /* SJF */
  void process_arrive_sjf(unsigned int current_time);
  void switch_out_cpu_sjf(unsigned int current_time);
  void complete_io_sjf(unsigned int current_time);
  void start_cpu_use_sjf(unsigned int current_time);
  void shortest_job_first();
  
  /* SRT */
  void process_arrive_srt(unsigned int current_time);
  void switch_out_cpu_srt(unsigned int current_time);
  void complete_io_srt(unsigned int current_time);
  void start_cpu_use_srt(unsigned int current_time);
  void shortest_remaining_time();
  
  /* RR */
  void process_arrive_rr(unsigned int current_time);
  void switch_out_cpu_rr(unsigned int current_time);
  void complete_io_rr(unsigned int current_time);
  void start_cpu_use_rr(unsigned int current_time);
  void ts_expiration_rr(unsigned int current_time);
  void round_robin();
};

struct Action
{
  int time;
  void (OpSys::*func)(unsigned int);
};

class CompAction
{
public:
  bool operator() (Action a, Action b)
  {
    return a.time > b.time;
  }
};

#endif
