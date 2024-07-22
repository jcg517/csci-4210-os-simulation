#ifndef __OPSYS_H__
#define __OPSYS_H__

#include <iostream>
#include <vector>
#include <queue>
#include <unordered_set>
using namespace std;

struct
{
  char* id;
  bool is_cpu_bound;
  int burst_index;
  std::vector<double> cpu_bursts;
  std::vector<double> io_bursts;
} Process;

class OpSys
{
public:
  Process* running;
  std::queue<Process*> ready;
  std::unordered_set<Process*> waiting;
};

#endif
