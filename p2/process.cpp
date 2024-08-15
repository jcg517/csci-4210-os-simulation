#include "process.h"


void Process::update() 
{
	this->burst_index++;

	if (onCPUBurst()) 
	{
		this->prev_t = t;
		this->prev_tau = tau;
		this->t = burst_times[burst_index];
		this->tau = (alpha * prev_t) + (alpha * prev_tau);

	} 
	else //onIOBurst
	{

	}
}


void Process::preempt( int elapsed_time )
 {
	int time_remaining = burst_times[burst_index] - elapsed_time;
	burst_times[burst_index] = time_remaining;
	this->t = time_remaining;
	this->tau -= elapsed_time;
} 
