#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <cstring>



double nextExp(double lambda)
{
    return -log(drand48()) / lambda;
}

int main(int argc, char* argv[])
{
    if (argc != 6) 
    {
		std::cerr << "ERROR: <error-text-here>\n";
		exit(1);
	}

    int n = *(argv+1);          /* # of processes to simulate */
    int n_cpu = *(argv+2);      /* # of CPU-bound processes */
    int seed = *(argv+3);       /* Rand num gen seed */
    double lambda = *(argv+4);     
    int ceil = *(argv+5);

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
        char p_name[2]; 
        std::sprintf(p_name, "%c%d", 'A' + (n / 10), n % 10);
        
        // Process arrival time
        int p_arrival_time = floor(nextExp(lambda));
        
        // Burst times
        int num_cpu = ceil(drand48() * 32);      // # CPU bursts
        int num_io = num_cpu - 1;
        int num_total = num_cpu + num_io;
        int[num_total] burst_times = {};

        // fill combined burst times list
        for (int j = 0; j < num_total; j++)
        {
            int x = ceil(nextExp(lambda));

            if (j % 2 == 0) x = (is_cpu_bound) ? x*4 : x;
            else x = (is_cpu_bound) ? x : x*8;
            
            burst_times[j] = x;
        }

        

    }

}


// num processes
