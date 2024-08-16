#include "process.h"

void Process::update() 
{
  this->burst_index++;

	if (onCPUBurst()) 
	{
		this->prev_t = t;
		this->t = burst_times[burst_index];
	}
	else //onIOBurst
	{
		this->prev_tau = tau;
		this->tau = std::ceil((alpha * burst_times[burst_index-1]) + ((1.0-alpha) * prev_tau));
	}
}

void Process::preempt( int elapsed_time )
{
	int time_remaining = burst_times[burst_index] - elapsed_time;
	burst_times[burst_index] = time_remaining;
	this->t = time_remaining;
	this->tau -= elapsed_time;
} 

void Process::reset()
{
	this->burst_index = 0;
	this->burst_completion_time = 0;
	this->t = burst_times[0];
	this->prev_t = t;
	this->tau = tau_0;
	this->prev_tau = tau;
}
