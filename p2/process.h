#ifndef __PROCESS_H__
#define __PROCESS_H__

class Process {
private:
  int burst_completion_time = 0;
public:
  char * id;
  int arrival_time;
  int * burst_times;
  int cpu_burst_count;
  int burst_index = 0;

  bool onCPUBurst() { return burst_index % 2 == 0; };
  bool onIOBurst() { return burst_index % 2 != 0; };
  int currentBurstTime() { return burst_times[burst_index]; };

  int waitBurst(int current_time) { return burst_completion_time = current_time + burst_times[burst_index]; };
  int burstCompletionTime() { return burst_completion_time };
  void finishBurst() { burst_completion_time = 0; };
};

#endif
