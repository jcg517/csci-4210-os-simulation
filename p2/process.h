#ifndef __PROCESS_H__
#define __PROCESS_H__

class Process {
public:
  char * id;
  int arrival_time;
  int * burst_times;
  int cpu_burst_count;
  int burst_index = 0;

  bool onCPUBurst() { return burst_index % 2 == 0; };
  bool onIOBurst() { return burst_index % 2 != 0; };

  
};

#endif
