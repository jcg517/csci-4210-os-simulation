#ifndef __OPSYS_H__
#define __OPSYS_H__

#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <queue>
#include <unordered_set>
#include <cstring>
#include "process.h"

#define TRUNCATE false 
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

class CompPredBurstRemTime
{
public:
  bool operator() (Process* a, Process* b)
  {
    if (a->tau_remaining == b->tau_remaining)
    {
      return std::strcmp(a->id, b->id) > 0;
    }
    return a->tau_remaining > b->tau_remaining;
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
template<class T> void print_priority_queue(const std::priority_queue<Process*, std::vector<Process*>, T> &ready)
{
  std::priority_queue<Process*, std::vector<Process*>, T> q = ready;
  std::cout << "[Q";
  if (!q.empty())
  {
    while (!q.empty())
    {
      std::cout << " " << q.top()->id;
      q.pop();
    }
  } else
  {
    std::cout << " empty";
  }
  std::cout << "]\n";
};

class OpSys
{
public:
  Process* running = NULL;
  Process* switching_to_run = NULL;
  Process* switching_to_ready = NULL;
  Process* switching_to_io = NULL;
  std::queue<Process*> ready_fcfs;
  std::priority_queue<Process*, std::vector<Process*>, CompPredBurstTime> ready_sjf;
  std::priority_queue<Process*, std::vector<Process*>, CompPredBurstRemTime> ready_srt;
  std::queue<Process*> ready_rr; 
  std::priority_queue<Process*, std::vector<Process*>, CompBurstCompletionTime> waiting;
  std::priority_queue<Process*, std::vector<Process*>, CompArrivalTime> unarrived;
  std::unordered_set<Process*> unfinished;
  std::vector<Process*> finished;
  unsigned int time = 0;
  int t_cs;
  int tslice;
  
  void finish_io_switch_out(unsigned int current_time) { switching_to_io = NULL; if (current_time == 0) return; };
  
  /* FCFS */
  void process_arrive_fcfs(unsigned int current_time);
  void switch_out_cpu_fcfs(unsigned int current_time);
  void complete_io_fcfs(unsigned int current_time);
  void start_cpu_use_fcfs(unsigned int current_time);
  void start_switch_in_fcfs(unsigned int current_time);
  void first_come_first_served();
  
  /* SJF */
  void process_arrive_sjf(unsigned int current_time);
  void switch_out_cpu_sjf(unsigned int current_time);
  void complete_io_sjf(unsigned int current_time);
  void start_cpu_use_sjf(unsigned int current_time);
  void start_switch_in_sjf(unsigned int current_time);
  void shortest_job_first();
  
  /* SRT */
  void process_arrive_srt(unsigned int current_time);
  void switch_out_cpu_srt(unsigned int current_time);
  void complete_io_srt(unsigned int current_time);
  void start_cpu_use_srt(unsigned int current_time);
  void start_switch_in_srt(unsigned int current_time);
  void finish_preempt_switch_out_srt(unsigned int current_time);
  void shortest_remaining_time();
  
  /* RR */
  void process_arrive_rr(unsigned int current_time);
  void switch_out_cpu_rr(unsigned int current_time);
  void complete_io_rr(unsigned int current_time);
  void start_cpu_use_rr(unsigned int current_time);
  void ts_expiration_rr(unsigned int current_time);
  void start_switch_in_rr(unsigned int current_time);
  void finish_preempt_switch_out_rr(unsigned int current_time);
  void round_robin();

  /* Statistics */
  void stats(std::ofstream& simout);
  void stats_rr(std::ofstream& simout);


};

struct Action
{
  unsigned int time;
  void (OpSys::*func)(unsigned int);
  int priority;
};

class CompAction
{
public:
  bool operator() (Action a, Action b)
  {
    if (a.time == b.time)
    {
      return a.priority > b.priority;
    }
    
    return a.time > b.time;
  }
};

#endif
