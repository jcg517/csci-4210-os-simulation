#include "opsys.h"

void print_priority_queue(const std::priority_queue<Process*, std::vector<Process*>, CompPredBurstTime> &ready)
{
  std::priority_queue<Process*, std::vector<Process*>, CompPredBurstTime> q = ready;
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
}

void OpSys::process_arrive_sjf( unsigned int current_time )
{
  Process* p = unarrived.top();
  unarrived.pop();
  ready_sjf.push(p);
  if (!TRUNCATE || current_time<TRUNC_TIME)
  {
    std::cout << "time " << current_time << "ms: Process " << p->id << " (tau " << p->getTau() << "ms) arrived; added to ready queue ";
    print_priority_queue(ready_sjf);
  }
}

void OpSys::start_cpu_use_sjf( unsigned int current_time )
{
  Process* p = ready_sjf.top();
  ready_sjf.pop();
  this->running = p;
  p->waitBurst(current_time);
  if (!TRUNCATE || current_time<TRUNC_TIME)
  {
    std::cout << "time " << current_time << "ms: Process " << p->id << " (tau " << p->getTau() << "ms) started using the CPU for " << p->getT() << "ms burst ";
    print_priority_queue(ready_sjf);
  }
}

void OpSys::switch_out_cpu_sjf( unsigned int current_time )
{
  Process* p = running;
  running = NULL;
  int old_tau = p->getTau();
  p->update();
  int bursts_left = p->getCpuBurstsLeft();
  if (bursts_left == 0)
  {
    std::cout << "time " << current_time << "ms: Process " << p->id << " terminated ";
    unfinished.erase(p);
    print_priority_queue(ready_sjf);
  } else
  {
    p->waitBurst(current_time+t_cs/2);
    if (!TRUNCATE || current_time<TRUNC_TIME)
    {
      std::cout << "time " << current_time << "ms: Process " << p->id << " (tau " << p->getTau() << "ms) completed a CPU burst; " << bursts_left << " burst" << (bursts_left == 1 ? "" : "s")  << " to go ";
      print_priority_queue(ready_sjf);
      /* maybe move below to another function? */
      std::cout << "time " << current_time << "ms: Recalculated tau for process " << p->id << ": old tau " << old_tau << "ms ==> new tau " << p->getTau() << "ms ";
      print_priority_queue(ready_sjf);
      std::cout << "time " << current_time << "ms: Process " << p->id << " (tau " << p->getTau() << "ms) switching out of CPU; blocking on I/O until time " << p->burstCompletionTime() << "ms ";
      print_priority_queue(ready_sjf);
    }
    waiting.push(p);
  }
}

void OpSys::complete_io_sjf( unsigned int current_time )
{
  Process* p = waiting.top();
  waiting.pop();
  p->update();
  ready_sjf.push(p);
  if (!TRUNCATE || current_time<TRUNC_TIME)
  {
    std::cout << "time " << current_time << "ms: Process " << p->id << " (tau " << p->getTau() << "ms) completed I/O; added to ready queue ";
    print_priority_queue(ready_sjf);
  }
  p->finishBurst();
}

void OpSys::shortest_job_first()
{
  bool switch_wait = false;
  this->time = 0;
  std::cout << "time " << this->time << "ms: Simulator started for SJF [Q empty]\n";
  while (!this->unfinished.empty())
  {
    /* Search for next 'interesting' event. */
    std::priority_queue<Action, std::vector<Action>, CompAction> action_queue;
 
    /* See if CPU isn't doing anything. */
    if (running == NULL)
    {
      if (!ready_sjf.empty())
        action_queue.push( { time+t_cs/2+(switch_wait ? t_cs/2 : 0), &OpSys::start_cpu_use_sjf } );
      switch_wait = false;
    } else
    
    /* See if running process is done. */
    {
      action_queue.push( { running->burstCompletionTime(), &OpSys::switch_out_cpu_sjf } );
      switch_wait = true;
    }

    /* Check if soonest IO burst is done. */
    if (!waiting.empty())
    {
      action_queue.push( { waiting.top()->burstCompletionTime(), &OpSys::complete_io_sjf } );
    }

    /* Check for incoming processes. */
    if (!unarrived.empty())
    {
      action_queue.push( {unarrived.top()->arrival_time, &OpSys::process_arrive_sjf } );
    }

    this->time = action_queue.top().time;
    (this->*(action_queue.top().func))(this->time);

  }
  std::cout << "time " << (this->time+t_cs/2) << "ms: Simulator ended for SJF [Q empty]\n";    
}
