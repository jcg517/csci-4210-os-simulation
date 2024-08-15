#include "opsys.h"

struct Action
{
  int time;
  void (*func)(unsigned int);
};

class CompAction
{
public:
  bool operator() (Action a, Action b)
  {
    return a.time > b.time;
  }
};

void OpSys::print_queue(const queue<Process*> &ready)
{
  queue<Process*> q = ready;
  std::cout << "[Q ";
  if (!q.empty())
  {
    while (!q.empty())
    {
      std::cout << q.front()->id << " ";
      q.pop();
    }
  } else
  {
    std::cout << "empty";
  }
  std::cout << "]\n";
}

void OpSys::process_arrive( unsigned int current_time )
{
  Process* p = unarrived.top();
  unarrived.pop();
  ready_fcfs.push(p);
  std::cout << "time " << p->arrival_time << "ms: Process " << p->id << " arrived; added to ready queue ";
  print_queue(ready_fcfs);
}

void OpSys::start_cpu_use( unsigned int current_time )
{
  Process* p = ready_fcfs.front();
  ready_fcfs.pop();
  this->running = p;
  p->waitBurst(current_time);
  std::cout << "time " << current_time << "ms: Process " << p->id << " started using the CPU for " << p->getT() << "ms burst ";
  print_queue(ready_fcfs);
}

void OpSys::switch_out_cpu( unsigned int current_time )
{
  Process* p = running;
  running = NULL;
  p->update();
  if (p->calcCpuBurstsLeft() == 0)
  {
    std::cout << "time " << p->burstCompletionTime() << "ms: Process " << p->id << " terminated ";
    unfinished.erase(p);
  } else
  {
    std::cout << "time " << p->burstCompletionTime() << "ms: Process " << p->id << " completed a CPU burst; " << p->calcCpuBurstsLeft() << " bursts to go ";
    print_queue(ready_fcfs);
    /* maybe move below to another function? */
    std::cout << "time " << p->burstCompletionTime() << "ms: Process " << p->id << " switching out of CPU; blocking on I/O until time ";
    std::cout << p->waitBurst(p->burstCompletionTime()) << "ms ";
    waiting.push(p);
  print_queue(ready_fcfs);
}

void OpSys::complete_io( unsigned int current_time )
{
  Process* p = waiting.top();
  waiting.pop();
  p->update();
  ready_fcfs.push(p);
  std::cout << "time " << p->burstCompletionTime() << "ms: Process " << p->id << " completed I/O; added to ready queue ";
  print_queue(ready_fcfs);
  p->finishBurst();
}

void OpSys::first_come_first_served()
{
  bool switch_wait = false;
  this->time = 0;
  std::cout << "time " << this->time << "ms: Simulator started for FCFS [Q empty]\n";
  while (!this->unfinished.empty())
  {
    /* Search for next 'interesting' event. */
    std::priority_queue<Action, vector<Action>, CompAction> action_queue;
 
    /* See if CPU isn't doing anything. */
    if (running == NULL)
    {
      if (!ready_fcfs.empty())
        action_queue.push( { time+t_cs/2+(switch_wait ? t_cs/2 : 0), start_cpu_use } );
      switch_wait = false;
    } else
    
    /* See if running process is done. */
    {
      action_queue.push( { running->burstCompletionTime(), switch_out_cpu } );
      switch_wait = true;
    }

    /* Check if soonest IO burst is done. */
    if (!waiting.empty())
    {
      action_queue.push( { waiting.top()->burstCompletionTime(), complete_io } );
    }

    /* Check for incoming processes. */
    if (!unarrived.empty())
    {
      action_queue.push( {unarrived.top()->arrival_time, process_arrive } );
    }

    this->time = action_queue.top().time;
    action_queue.top().func(this->time);
  }
  std::cout << "time " << (this->time+t_cs/2) << "ms: Simulator ended for FCFS [Q empty]\n"    
}
