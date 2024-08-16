#include "process.h"


void Process::update() 
{
	this->burst_index++;
	this->time_remaining = 0;

	if (onCPUBurst()) 
	{
		this->prev_t = t;
		this->prev_tau = tau;
		this->t = burst_times[burst_index];
		this->tau = (alpha * prev_t) + ((1.0-alpha) * prev_tau);

	} 
	else //onIOBurst
	{

	}
}


void Process::preempt( int elapsed_time )
{
	int time_remaining = burst_times[burst_index] - elapsed_time;
	burst_times[burst_index] = time_remaining;
	// this->t = time_remaining;
	this->tau -= elapsed_time;
	this->time_remaining = time_remaining;
} 

void Process::reset()
{
	this->burst_index = 0;
	this->burst_completion_time = 0;
	this->t = burst_times[0];
	this->prev_t = t;
	this->tau = tau_0;
	this->prev_tau = tau;
	this->time_remaining = 0;
}
