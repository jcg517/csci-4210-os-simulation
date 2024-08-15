#include "opsys.h"

void OpSys::first_come_first_served()
{
  this->time = 0;
  std::cout << "time " << this->time << "ms: Simulator started for FCFS [Q empty]\n";
  while (!this->unarrived.empty())
  {
    /* Search for next 'interesting' event. */
    Process* next_arrival = this->unarrived.top();
    Process* next_io_rearrival = this->waiting.empty() ? NULL : this->waiting.top();
    Process* running = this->running;
}
