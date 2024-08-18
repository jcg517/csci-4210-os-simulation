#ifndef __PROCESS_H__
#define __PROCESS_H__

#include <cmath>

class Process {
public:
  unsigned int burst_completion_time = 0;
  int burst_index = 0;
  int prev_t;
  int prev_tau; 
  double alpha;
  int tau;
  int tau_0;
  int t;
  char * id;
  unsigned int * burst_times;
  unsigned int arrival_time;
  bool is_cpu_bound;
  int num_cpu_bursts;
  int num_total_bursts;
  unsigned int last_cpu_burst_start; 
  int time_remaining = 0;
  int tau_remaining = 0;
  unsigned int last_switch_time = 0;
  /* Statistics */
  int num_switches = 1;
  int num_preempts = 0;
  int total_cpu_time;
  int start_turnaround; 
  int total_turnaround; 

  bool onCPUBurst() { return burst_index % 2 == 0; };
  bool onIOBurst() { return burst_index % 2 != 0; };

  int getT() { return t; };
  int getTau() { return tau; };
  int getCpuBurstsLeft() { return std::ceil( (num_total_bursts - burst_index ) / 2 ); };
  int getTotalCpuTime();

  unsigned int waitBurst(unsigned int current_time) { return burst_completion_time = current_time + time_remaining; };
  unsigned int burstCompletionTime() { return burst_completion_time; };

  void finishBurst() { burst_completion_time = 0; };
  void update( int current_time ); // Increments burst index
  void preempt( unsigned int elapsed_time );

  void reset();
};

#endif
