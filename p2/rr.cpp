#include "opsys.h"

void OpSys::process_arrive_rr( unsigned int current_time )
{
  Process* p = unarrived.top();
  unarrived.pop();
  ready_rr.push(p);
  if (!TRUNCATE || current_time<TRUNC_TIME)
  {
    std::cout << "time " << current_time << "ms: Process " << p->id << " arrived; added to ready queue ";
    print_queue(ready_rr);
  }
}

void OpSys::start_cpu_use_rr( unsigned int current_time )
{
  Process* p = switching_to_run;
  this->switching_to_run = NULL;
  this->running = p;
	p->last_cpu_burst_start = current_time;
  p->waitBurst(current_time);
  if (!TRUNCATE || current_time<TRUNC_TIME)
  {
    if (p->time_remaining < p->getT())
    {
      std::cout << "time " << current_time << "ms: Process " << p->id << " started using the CPU for remaining " << p->time_remaining << "ms of " << p->getT() << "ms burst ";
    } else 
    {
      std::cout << "time " << current_time << "ms: Process " << p->id << " started using the CPU for " << p->getT() << "ms burst ";
    }
    print_queue(ready_rr);
  }
}

void OpSys::switch_out_cpu_rr( unsigned int current_time )
{
  Process* p = running;
  running = NULL;
  p->time_remaining = 0;
  p->update();
  int bursts_left = p->getCpuBurstsLeft();
  if (bursts_left == 0)
  {
    std::cout << "time " << current_time << "ms: Process " << p->id << " terminated ";
    unfinished.erase(p);
    print_queue(ready_rr);
  } else
  {
    p->waitBurst(current_time+t_cs/2);
    if (!TRUNCATE || current_time<TRUNC_TIME)
    {
      std::cout << "time " << current_time << "ms: Process " << p->id << " completed a CPU burst; " << bursts_left << " burst" << (bursts_left == 1 ? "" : "s")  << " to go ";
      print_queue(ready_rr);
      /* maybe move below to another function? */
      std::cout << "time " << current_time << "ms: Process " << p->id << " switching out of CPU; blocking on I/O until time " << p->burstCompletionTime() << "ms ";
      print_queue(ready_rr);
    }
    waiting.push(p);
  }
}

void OpSys::complete_io_rr( unsigned int current_time )
{
  Process* p = waiting.top();
  waiting.pop();
  p->update();
  ready_rr.push(p);
  if (!TRUNCATE || current_time<TRUNC_TIME)
  {
    std::cout << "time " << current_time << "ms: Process " << p->id << " completed I/O; added to ready queue ";
    print_queue(ready_rr);
  }
  p->finishBurst();
}


void OpSys::ts_expiration_rr(unsigned int current_time)
{
	Process* p = running;
	/* If ready queue is empty, there is no preemption */
	if (ready_rr.empty())
	{
		p->time_remaining -= tslice;
    if (!TRUNCATE || current_time<TRUNC_TIME)
      std::cout << "time " << current_time << "ms: Time slice expired; no preemption because ready queue is empty [Q empty]\n";
		p->last_cpu_burst_start = current_time;
		return;
	}

	p->preempt(tslice);
	running = NULL;
	if (!TRUNCATE || current_time<TRUNC_TIME)
  {
    std::cout << "time " << current_time << "ms: Time slice expired; preempting process " <<  p->id << " with " << p->time_remaining << "ms remaining ";
	  print_queue(ready_rr);
	}	
  ready_rr.push(p);
}

void OpSys::round_robin()
{
  bool switch_wait = false;
  this->time = 0;
  std::cout << "time " << this->time << "ms: Simulator started for RR [Q empty]\n";
  while (!this->unfinished.empty())
  {
    /* Search for next 'interesting' event. */
    std::priority_queue<Action, std::vector<Action>, CompAction> action_queue;
 
    /* See if CPU isn't doing anything. */
    if (running == NULL)
    {
      if (!ready_rr.empty())
      {
        if (switching_to_run == NULL)
        {
          switching_to_run = ready_rr.front();
          ready_rr.pop();
          switching_to_run->last_switch_time=this->time;
        }

        action_queue.push( { switching_to_run->last_switch_time+t_cs/2+(switch_wait ? t_cs/2 : 0), &OpSys::start_cpu_use_rr } );
      }
      switch_wait = false;
    } else
    
    /* See if running process is done OR check for preemption. */
    {
      if ((running->last_cpu_burst_start + tslice) < running->burstCompletionTime() )
			{
				action_queue.push( { (running->last_cpu_burst_start + tslice), &OpSys::ts_expiration_rr } );
			} else
      {
        action_queue.push( { running->burstCompletionTime(), &OpSys::switch_out_cpu_rr } );
      }
      switch_wait = true;
    }

    /* Check if soonest IO burst is done. */
    if (!waiting.empty())
    {
      action_queue.push( { waiting.top()->burstCompletionTime(), &OpSys::complete_io_rr } );
    }

    /* Check for incoming processes. */
    if (!unarrived.empty())
    {
      action_queue.push( {unarrived.top()->arrival_time, &OpSys::process_arrive_rr } );
    }

    this->time = action_queue.top().time;
    (this->*(action_queue.top().func))(this->time);
  }

  std::cout << "time " << (this->time+t_cs/2) << "ms: Simulator ended for RR [Q empty]\n";    
}
