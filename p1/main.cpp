#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <cstring>

#include "opsys.h"


double nextExp(double lambda, int ceil)
{
    while (true)
    {
        double x = -log(drand48()) / lambda;
        if (x < ceil) return x;
    }
}

int main(int argc, char* argv[])
{
    if (argc != 6) 
    {
		std::cerr << "ERROR: <error-text-here>\n";
		exit(1);
	}

    int n = atoi(*(argv+1));          /* # of processes to simulate */
    int n_cpu = atoi(*(argv+2));      /* # of CPU-bound processes */
    int seed = atoi(*(argv+3));       /* Rand num gen seed */
    double lambda = atof(*(argv+4));     
    int ceil = atoi(*(argv+5));

    // Seed rand num generator
    srand48( seed );

    // Process creation loop
    int n_cpu_count = 0;

    for (int i = 0; i < n; i++)
    {
        // Determine process type:
        bool is_cpu_bound = false;
        if (n_cpu_count < n_cpu) 
        {
            is_cpu_bound = true;
            n_cpu_count++;
        }

        // Process name
        char p_name[3]; 
        std::sprintf(p_name, "%c%d", 'A' + (n / 10), n % 10);
        
        // Process arrival time
        int p_arrival_time = floor(nextExp(lambda, ceil));
        
        // Burst times
        int num_cpu = std::ceil(drand48() * 32);      // # CPU bursts
        int num_io = num_cpu - 1;
        int num_total = num_cpu + num_io;
        int* burst_times = new int[num_total];

        // fill combined burst times list
        for (int j = 0; j < num_total; j++)
        {
            int x = std::ceil(nextExp(lambda, ceil));

            if (j % 2 == 0) x = (is_cpu_bound) ? x*4 : x;
            else x = (is_cpu_bound) ? x : x*8;
            
            burst_times[j] = x;
        }

        Process* p = new Process();
        p->id = p_name;
        p->is_cpu_bound = is_cpu_bound;
        p->num_cpu = num_cpu;
        p->burst_index = 0;
        p->burst_times = burst_times;
        p->arrival_time = p_arrival_time;
    }  

}


// num processes
