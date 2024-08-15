#include "opsys.h"

void OpSys::shortest_job_first()
{
  this->time = 0;
  std::cout << "time " << this->time << "ms: Simulator started for SJF [Q empty]\n";

  while (!this->unarrived.empty())
  {
    /* Search for next 'interesting' event. */

		/* Check for process arrival Process arrival */
		Process* nextProcess = unarrived.top();
		if (nextProcess->arrival_time == time)
		{
			
		}


  }
}