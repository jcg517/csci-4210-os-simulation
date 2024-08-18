#ifndef __OPSYS_H__
#define __OPSYS_H__

#include <iostream>
#include <vector>
#include <queue>
#include <unordered_set>
#include <cmath>

struct Process
{
  char* id;
  bool is_cpu_bound;
  int num_cpu;
  int* burst_times;
  int burst_index;
  int arrival_time;
};

class OpSys
{
public:
  Process* running;
  std::queue<Process*> ready;
  std::unordered_set<Process*> waiting;
};

#endif
